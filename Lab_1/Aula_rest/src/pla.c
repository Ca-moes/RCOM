// Protocolo de Ligação de Dados
#include "pla.h"


int readingCycle(enum readingType type, int fd, unsigned char *c, unsigned char **dataBuf, int *retBufferSize){
  volatile int STOP=FALSE;
  int res;
  unsigned char buf[1];

  while (STOP==FALSE) {       /* loop for input */
    res = read(fd,buf,1);   /* returns after 1 char has been input */

    if (type == closeUA)
    {
      if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
      log_caution("receiver_DISC_UA() - Failed reading UA from transmitter.");
      return -1;
      } else if (res == -1){
      log_error("receiver_DISC_UA() - Failed reading UA from buffer.");
      return -1;}
    }
    else
    {
      if (res == -1) {
        switch (type)
        {
        case openR:
          log_error("receiver: failed reading SET from buffer.");
          break;
        case readR:
          log_error("llread() - Failed reading frame from buffer.");
          break;
        case closeDISC:
          log_error("receiver_DISC_UA() - Failed reading DISC from buffer.");
          break;
        default:
          log_error("readingCycle() - Unknown type");
          break;
        }
      log_error("readingCycle() - Error on read()");
      return -1;
      }
    }
    
    if (type == readR)
    {
      int retStateMachine = stateMachine(buf[0], dataBuf, retBufferSize);
      if (retStateMachine == -1){
        *c = C_REJ(linkLayer.sequenceNumber);
        log_error("llread() - Error in BCC");
        return -1;
      }
      if (retStateMachine == -2){
        *c = C_RR(linkLayer.sequenceNumber);
        log_error("llread() - Error in C, wrong sequence number");
        break;
      }
      *c = C_RR(linkLayer.sequenceNumber);
    }
    else
      stateMachine(buf[0], NULL, NULL);

    if (state_machine.state == DONE) STOP=TRUE;
  }
  return 0;
}

int writeCycle(enum writingType type, int fd, unsigned char *buf, int bufsize){
  int attempt = 0, res;
  volatile int STOP=FALSE;

  do{
    attempt++;
    res = write(fd,buf,bufsize);
    if (res == -1) {
      switch (type)
      {
      case trans_SET:
        log_error("transmitter_SET() - Failed writing SET to buffer.");
        break;
      case writeR:
        log_error("llwrite() - Failed writing data to buffer.");
        break;
      case trans_DISC_UA:
        log_error("transmitter_DISC_UA() - Failed writing DISC to buffer.");
        break;
      default:
        log_error("writeCycle() - Unknown type");
        return -1;
        break;
      }
      return -1;
    }

    alarm(linkLayer.timeout);
    failed = FALSE;
    state_machine.state = Start; 
    unsigned char buf_r[1];
    
    while (STOP==FALSE) { 
      res = read(fd,buf_r,1);   /* returns after 1 char has been input */

      if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
        switch (type)
        {
        case trans_SET:
          log_caution("transmitter_SET - Failed reading UA from receiver.");
          break;
        case writeR:
          log_caution("llwrite: failed reading RR from receiver.");
          break;
        case trans_DISC_UA:
          log_caution("transmitter_DISC_UA() - Failed reading DISC from receiver.");
          break;
        default:
          log_error("writeCycle() - Unknown type");
          return -1;
          break;
        }
        if (attempt < linkLayer.numTransmissions)
          log_caution("Trying again...");
        else{
          log_error("writeCycle() - Exceded number of attempts");
          return -1;
        }
        break;
      } else if (res == -1){
        switch (type)
        {
        case trans_SET:
          log_error("transmitter_SET() - Failed reading UA from buffer.");
          break;
        case writeR:
          log_error("llwrite() - Failed reading RR from buffer.");
          break;
        case trans_DISC_UA:
          log_error("transmitter_DISC_UA() - Failed reading DISC from buffer.");
          break;
        default:
          log_error("writeCycle() - Unknown type");
          return -1;
          break;
        }
        log_error("writeCycle() - Error on read()");
        return -1;
      }

      if (type == writeR){
        if (stateMachine(buf_r[0], NULL, NULL) < 0){
          log_error("llwrite() - Error while receiving RR or REJ");
          failed = TRUE;
          alarm(0);
          break;
        }
      } else {
        stateMachine(buf_r[0], NULL, NULL);
      }
    
      if (state_machine.state == DONE || failed) STOP=TRUE;
    }
  }while (attempt < linkLayer.numTransmissions && failed);
  alarm(0);
  return 0;
}


int initConnection(int *fd, char *port){
  struct termios newtio;

  *fd = open(port, O_RDWR | O_NOCTTY );
  if (fd < 0) {perror(port); exit(-1); }

  if ( tcgetattr(*fd,&oldtio) == -1) { /* save current port settings */
    log_error("initConnection() - Error on tcgetattr - check cable connection or port number");
    return -1;
  }

  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
  /* set input mode (non-canonical, no echo,...) */
  newtio.c_lflag = 0;
  newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 1;   /* blocking read until 5 chars received */

  tcflush(*fd, TCIOFLUSH);

  if ( tcsetattr(*fd,TCSANOW,&newtio) == -1) {
    perror("tcsetattr");
    exit(-1);
  }

  strcpy(linkLayer.port,port);
  linkLayer.baudRate = BAUDRATE;
  linkLayer.sequenceNumber = 0x00;
  linkLayer.timeout = 3;
  linkLayer.numTransmissions = 3;

  log_success("New termios structure set");
  return 0;
}

void atende(){ 
  if (state_machine.state != DONE){
    failed = TRUE;
    log_caution("Alarm Activated");
    return;
  }
}

int transmitter_SET(int fd){
  unsigned char buf[SU_FRAME_SIZE] = {FLAG, A_ER, C_SET, BCC(A_ER, C_SET), FLAG};

  stateMachineSetUp(C_UA, A_ER, Start, Supervision);

  writeCycle(trans_SET, fd, buf, SU_FRAME_SIZE);
  
  if(failed == TRUE){
    log_error("transmitter_SET() - Failed all attempts");
    return -1;
  } 
  return fd;
}

int receiver_UA(int fd){
  stateMachineSetUp(C_SET, A_ER, Start, Supervision);


  if (readingCycle(openR, fd, NULL, NULL, NULL) < 0)
    return -1;

  unsigned char replyBuf[SU_FRAME_SIZE] = {FLAG, A_ER, C_UA, BCC(A_ER, C_UA), FLAG};

  int res = write(fd,replyBuf,SU_FRAME_SIZE); //+1 para enviar o \0 
  if (res == -1) {
    log_error("receiver_UA() - Failed writing UA to buffer.");
    return -1;}
  
  return fd;
}

int llopen(int porta, int type){
  int fd;
  char port[12]; // "/dev/ttyS11\0" <- 12 chars
  snprintf(port, 12, "/dev/ttyS%d", porta);
  
  if (initConnection(&fd, port) < 0){
    log_error("llopen() - Error on initConnection");
    return -1;
  }
  
  // Set Handler for Alarm
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = atende;
  sa.sa_flags = 0;
  sigaction(SIGALRM, &sa, NULL);

  switch (type)
  {
  case TRANSMITTER:
    linkLayer.status = TRANSMITTER;
    return transmitter_SET(fd);
    break;
  case RECEIVER:
    linkLayer.status = RECEIVER; 
    return receiver_UA(fd);
    break;
  default:
    log_error("llopen() - Wrong value for variable type");
    return -1;
    break;
  }

}


int fillFinalBuffer(unsigned char* finalBuffer, unsigned char* headerBuf, unsigned char* footerBuf, unsigned char* dataBuffer, int dataSize){
  int finalIndex = 0, dataIndex = 0, footerBufIndex=0;

  while (finalIndex < 4){
  finalBuffer[finalIndex] = headerBuf[finalIndex];
  finalIndex++;
  }
  while (dataIndex < dataSize){
    finalBuffer[finalIndex] = dataBuffer[dataIndex];
    finalIndex++; dataIndex++;
  }
  while (footerBufIndex < 2){
    finalBuffer[finalIndex] = footerBuf[footerBufIndex];
    finalIndex++; footerBufIndex++;
  }

  return finalIndex;
}

int llwrite(int fd, char *buffer, int lenght){
  int currentLenght = lenght;
  unsigned char buf1[4] = {FLAG, A_ER, C_I(linkLayer.sequenceNumber), BCC(A_ER, C_I(linkLayer.sequenceNumber))};  
  unsigned char *dataBuffer = (unsigned char *)malloc(lenght);

  if (lenght > MAX_SIZE){
    log_error("llwrite() - Message size greater than MAX_SIZE");
    return -1;
  }

  /*building trama I*/
  unsigned char BCC2 = buffer[0];
  for (int i = 1; i<lenght; i++){
    BCC2 = BCC2 ^ buffer[i];
  }

  unsigned char buf2[2] = {BCC2, FLAG};
  
  // Byte Stuffing
  for (int i = 0, k=0; i<lenght; i++, k++){
    if (buffer[i] == 0x7E || buffer[i] == 0x7D){
      currentLenght++;
      dataBuffer = (unsigned char *) realloc(dataBuffer, currentLenght); 

      dataBuffer[k+1] = buffer[i] ^ 0x20;
      dataBuffer[k] = 0x7D;
      k++;
    }
    else{
      dataBuffer[k] = buffer[i];
    }
  }

  unsigned char finalBuffer[currentLenght + 6]; /*trama I completa*/

  fillFinalBuffer(finalBuffer, buf1, buf2, dataBuffer, currentLenght);

  stateMachineSetUp(C_RR(linkLayer.sequenceNumber^0x01), A_ER, Start, Write);

  if (writeCycle(writeR, fd, finalBuffer, sizeof(finalBuffer)) <0){
    log_error("llwrite() - failed writing");
    return -1;
  }
  
  linkLayer.sequenceNumber ^= 0x01;

  return 0;
}

int llread(int fd, unsigned char *buffer){
  unsigned char *dataBuf;
  int retBufferSize;
  stateMachineSetUp(C_I(linkLayer.sequenceNumber), A_ER, Start, Read);

  unsigned char c;

  frameIndex = 0;
  if (readingCycle(readR, fd, &c, &dataBuf, &retBufferSize) < 0){
    log_error("llread() - failed reading");
    return -1;
  }

  unsigned char replyBuf[5] = {FLAG, A_ER, c, BCC(A_ER, c), FLAG};

  for (int i = 0; i < retBufferSize; i++)
    buffer[i] = dataBuf[i];
  
  int res = write(fd,replyBuf,5);
  if (res == -1) {
    log_error("llread() - Failed writing response to buffer.");
    return -1;}

  free(dataBuf);
  return retBufferSize; 
}


int transmitter_DISC_UA(int fd){
  unsigned char buf[SU_FRAME_SIZE] = {FLAG, A_ER, C_DISC, BCC(A_ER, C_DISC), FLAG};

  stateMachineSetUp(C_DISC, A_RE, Start, Supervision);

  writeCycle(trans_DISC_UA, fd, buf, SU_FRAME_SIZE);

  if(failed == TRUE){
    log_error("transmitter_DISC_UA() - Failed all attempts");
    return -1;
  } 

  unsigned char buf1[SU_FRAME_SIZE] = {FLAG, A_RE, C_UA, BCC(A_RE, C_UA), FLAG};

  int res = write(fd,buf1,SU_FRAME_SIZE);
  if (res == -1) {
    log_error("transmitter_DISC_UA() - Failed writing UA to buffer.");
    return -1;
  }

  return fd;
}

int receiver_DISC_UA(int fd){
  stateMachineSetUp(C_DISC, A_ER, Start, Supervision);
  
  /* parse DISC*/
  readingCycle(closeDISC, fd, NULL, NULL, NULL);
  
  unsigned char replyBuf[SU_FRAME_SIZE] = {FLAG, A_RE, C_DISC, BCC(A_RE, C_DISC), FLAG};

  int res = write(fd,replyBuf,SU_FRAME_SIZE); 
  if (res == -1) {
    log_error("receiver_DISC_UA() - Failed writing DISC to buffer.");
    return -1;
  }

  stateMachineSetUp(C_UA, A_RE, Start, Supervision);

  alarm(5); /* waits a limited time for UA response from Transmitter */
  /* parse UA*/
  if (readingCycle(closeDISC, fd, NULL, NULL, NULL) < 0)
    return -1;
  
  alarm(0);
  return fd;
}

int llclose(int fd){
  //sleep(1); /* give llwrite time to receive response*/
  int returnValue = fd;

  switch (linkLayer.status)
  {
  case TRANSMITTER:

    if (transmitter_DISC_UA(fd) <0)
      returnValue = -1;
    break;
  case RECEIVER:
    if (receiver_DISC_UA(fd) <0)
      returnValue = -1;
    break;
  }

  if (tcsetattr(fd,TCSANOW,&oldtio) != 0){
    log_error("llclose() - Error on tcsetattr()");
    return -1;
  }
  if (close(fd) != 0){
    log_error("llclose() - Error on close()");
    return -1;
  }

  if (returnValue>0)
    log_success("Closing Successful");

  return returnValue;
}