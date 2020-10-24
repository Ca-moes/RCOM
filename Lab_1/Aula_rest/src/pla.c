// Protocolo de Ligação de Dados
#include "pla.h"

struct termios oldtio; // utilizado para fechar a ligação em llclose
enum stateMachineState state;
int failed = FALSE;

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
  if (state != DONE){
    failed = TRUE;
    log_caution("Alarm Activated");
    return;
  }
}

int transmitter_SET(int fd){
  unsigned char buf[SU_FRAME_SIZE] = {FLAG, A_ER, C_SET, BCC(A_ER, C_SET), FLAG};
  unsigned char buf_read[1]; // read buffer
  int attempt = 0, res;
  volatile int STOP=FALSE;

  // Set-Up State Machine
  state_machine.control = C_UA;
  state_machine.address = A_ER;
  state_machine.state = Start;
  state_machine.type = Supervision;

  do{
    attempt++;
    res = write(fd,buf,SU_FRAME_SIZE);
    if (res == -1) {
      log_error("transmitter_SET() - Failed writing SET to buffer.");
      return -1;
    }
        
    alarm(linkLayer.timeout);
    failed = FALSE;
    state_machine.state = Start; 

    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf_read,1);   /* returns after 1 char has been input */

      if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
        log_caution("transmitter_SET - Failed reading UA from receiver.");
        if (attempt < linkLayer.numTransmissions) {
          log_caution("Trying again...");
          failed = TRUE;
        }
        break;
      } else if (res == -1){
        log_error("transmitter_SET() - Failed reading UA from buffer.");
        return -1;
      }
      
      stateMachine(buf_read[0], NULL, NULL);
      if (state_machine.state == DONE || failed) STOP=TRUE;
    }
  }while (attempt < linkLayer.numTransmissions && failed);

  alarm(0); // cancel pending alarms

  if(failed == TRUE){
    log_error("transmitter_SET() - Failed all attempts");
    return -1;
  } 
  return fd;
}

int receiver_UA(int fd){
  unsigned char buf[1];
  int res;
  volatile int STOP=FALSE;

  // Set-Up State Machine
  state_machine.control = C_SET;
  state_machine.address = A_ER;
  state_machine.state = Start;
  state_machine.type = Supervision;

  while (STOP==FALSE) {       /* loop for input */
    res = read(fd,buf,1);   /* returns after 1 char has been input */
    if (res == -1) {
      log_error("receiver: failed reading SET from buffer.");
      return -1;
    }

      stateMachine(buf[0], NULL, NULL);
    if (state_machine.state == DONE) STOP=TRUE;
  }

  unsigned char replyBuf[SU_FRAME_SIZE] = {FLAG, A_ER, C_UA, BCC(A_ER, C_UA), FLAG};

  res = write(fd,replyBuf,SU_FRAME_SIZE); //+1 para enviar o \0 
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
  int res;
  int currentLenght = lenght;
  unsigned char buf1[4] = {FLAG, A_ER, C_I(linkLayer.sequenceNumber), BCC(A_ER, C_I(linkLayer.sequenceNumber))};  
  unsigned char *dataBuffer = (unsigned char *)malloc(lenght);
  unsigned char buf_read[MAX_SIZE];
  volatile int STOP=FALSE;
  int attempt = 0;

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

  // Set-Up State Machine
  state_machine.control = C_RR(linkLayer.sequenceNumber^0x01);
  state_machine.address = A_ER;
  state_machine.state = Start;
  state_machine.type = Write;

  /*sending trama I*/
  do{
    attempt++;
    res = write(fd,finalBuffer,sizeof(finalBuffer));
    if (res == -1) {
      log_error("llwrite() - Failed writing data to buffer.");
      return -1;
    }
    
    alarm(linkLayer.timeout);
    failed = FALSE;
    state_machine.state = Start;

    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf_read,1);   /* returns after 1 char has been input */

      if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
        log_caution("llwrite: failed reading RR from receiver.");
        if (attempt < linkLayer.numTransmissions) {
          log_caution("Trying again...");
          failed = TRUE;
        }
        break;
      }
      else if (res == -1){
        log_error("llwrite() - Failed reading RR from buffer.");
        return -1;
      }

      if (-1 == stateMachine(buf_read[0], NULL, NULL)){
        log_error("llwrite() - Error while receiving RR or REJ");
        failed = TRUE;
        alarm(0);
        break;
      }

      if (state_machine.state == DONE || failed) STOP=TRUE;
    }
  }while (attempt < linkLayer.numTransmissions && failed);

  alarm(0);
  linkLayer.sequenceNumber ^= 0x01;

  return 0;
}

int llread(int fd, unsigned char *buffer){
  unsigned char buf[1];
  unsigned char *dataBuf;
  int res, retBufferSize, retStateMachine;
  volatile int STOP=FALSE;
  // Set-Up State Machine
  state_machine.control = C_I(linkLayer.sequenceNumber);
  state_machine.address = A_ER;
  state_machine.state = Start;
  state_machine.type = Read;

  unsigned char c;

  while (STOP==FALSE) {       
    res = read(fd,buf,1);   
    if (res == -1) {
      log_error("llread() - Failed reading frame from buffer.");
      return -1;
    }
    
    retStateMachine = stateMachine(buf[0], &dataBuf, &retBufferSize);
    if (retStateMachine == -1){
      c = C_REJ(linkLayer.sequenceNumber);
      log_error("llread() - Error in BCC");
      break;
    }
    if (retStateMachine == -2){
      c = C_RR(linkLayer.sequenceNumber);
      log_error("llread() - Error in C, wrong sequence number");
      break;
    }
    c = C_RR(linkLayer.sequenceNumber);
    if (state_machine.state == DONE) STOP=TRUE;
  }

  unsigned char replyBuf[5] = {FLAG, A_ER, c, BCC(A_ER, c), FLAG};

  for (int i = 0; i < retBufferSize; i++)
    buffer[i] = dataBuf[i];
  
  res = write(fd,replyBuf,5);
  if (res == -1) {
    log_error("llread() - Failed writing response to buffer.");
    return -1;
  }

  free(dataBuf);
  return retBufferSize; 
}


int transmitter_DISC_UA(int fd){
  unsigned char buf[SU_FRAME_SIZE] = {FLAG, A_ER, C_DISC, BCC(A_ER, C_DISC), FLAG};
  unsigned char buf_read[SU_FRAME_SIZE]; // read buffer
  int attempt = 0, res;
  volatile int STOP=FALSE;

  // Set-Up State Machine
  state_machine.control = C_DISC;
  state_machine.address = A_RE;
  state_machine.state = Start;
  state_machine.type = Supervision;

  do{
    attempt++;
    res = write(fd,buf,SU_FRAME_SIZE);
    if (res == -1) {
      log_error("transmitter_DISC_UA() - Failed writing DISC to buffer.");
      return -1;
    }
        
    alarm(linkLayer.timeout);
    failed = FALSE;
    state_machine.state = Start; 

    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf_read,1);   /* returns after 1 char has been input */

      if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
        log_caution("transmitter_DISC_UA() - Failed reading DISC from receiver.");
        if (attempt < linkLayer.numTransmissions) {
          log_caution("Trying again...");
        }
        break;
      } else if (res == -1){
        log_error("transmitter_DISC_UA() - Failed reading DISC from buffer.");
        return -1;
      }
      
      stateMachine(buf_read[0],NULL, NULL);
      
      if (state_machine.state == DONE || failed) STOP=TRUE;
    }
  }while (attempt < linkLayer.numTransmissions && failed);

  alarm(0); // cancel pending alarms

  if(failed == TRUE){
    log_error("transmitter_DISC_UA() - Failed all attempts");
    return -1;
  } 

  unsigned char buf1[SU_FRAME_SIZE] = {FLAG, A_RE, C_UA, BCC(A_RE, C_UA), FLAG};

  res = write(fd,buf1,SU_FRAME_SIZE);
  if (res == -1) {
    log_error("transmitter_DISC_UA() - Failed writing UA to buffer.");
    return -1;
  }

  return fd;
}

int receiver_DISC_UA(int fd){
  unsigned char buf[1];
  int res;
  volatile int STOP=FALSE;
  // Set-Up State Machine
  state_machine.control = C_DISC;
  state_machine.address = A_ER;
  state_machine.state = Start;
  state_machine.type = Supervision;
  
  /* parse DISC*/
  while (STOP==FALSE) {       /* loop for input */
    res = read(fd,buf,1);   /* returns after 1 char has been input */
    if (res == -1) {
      log_error("receiver_DISC_UA() - Failed reading DISC from buffer.");
      return -1;
    }
    
    stateMachine(buf[0], NULL, NULL);
    if (state_machine.state == DONE) STOP=TRUE;
  }

  unsigned char replyBuf[SU_FRAME_SIZE] = {FLAG, A_RE, C_DISC, BCC(A_RE, C_DISC), FLAG};

  res = write(fd,replyBuf,SU_FRAME_SIZE); 
  if (res == -1) {
    log_error("receiver_DISC_UA() - Failed writing DISC to buffer.");
    return -1;
  }

  unsigned char buf1[1];
  alarm(5); /* waits a limited time for UA response from Transmitter */
  STOP=FALSE;
  // Set-Up State Machine
  state_machine.control = C_UA;
  state_machine.address = A_RE;
  state_machine.state = Start;
  state_machine.type = Supervision;

  /* parse UA*/
  while (STOP==FALSE) {       /* loop for input */
    res = read(fd,buf1,1);   /* returns after 1 char has been input */
    
    if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
      log_caution("receiver_DISC_UA() - Failed reading UA from transmitter.");
      return -1;

    } else if (res == -1){
      log_error("receiver_DISC_UA() - Failed reading UA from buffer.");
      return -1;
    }
    
    stateMachine(buf1[0], NULL, NULL);
    if (state_machine.state == DONE) STOP=TRUE;
  }
  alarm(0);

  return fd;
}

int llclose(int fd){
  sleep(1); /* give llwrite time to receive response*/
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