// Protocolo de Ligação de Dados
#include "pla.h"

struct termios oldtio; // utilizado para fechar a ligação em llclose
enum stateMachine state;
int failed = FALSE;

struct linkLayer linkLayer;


int initConnection(int *fd, char *port){
  struct termios newtio;

  *fd = open(port, O_RDWR | O_NOCTTY );
  if (fd < 0) {perror(port); exit(-1); }

  if ( tcgetattr(*fd,&oldtio) == -1) { /* save current port settings */
    log_error("Error on tcgetattr - check cable connection");
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
  linkLayer.sequenceNumber = 0;
  linkLayer.timeout = 3;
  linkLayer.numTransmissions = 3;

  log_success("New termios structure set");
  return 0;
}

void atende(){ 
  if (state != DONE){
    failed = TRUE;
    return;
  }
}

void stateMachine_SET_UA(enum stateMachine *state, unsigned char *checkBuffer, char byte, int type){
  switch (*state)
  {
  case Start:
    if (byte == FLAG) *state = FLAG_RCV;    
    break;
  case FLAG_RCV:
    if (byte == A_ER) {
      *state = A_RCV;
      checkBuffer[0] = byte;}
    else if (byte!= FLAG)
      *state = Start;
    break;
  case A_RCV:; // ; aqui explicado -> https://stackoverflow.com/a/19830820
    int C_byte = (type) ? C_UA : C_SET; 
    if (byte == C_byte) {
      *state = C_RCV;
      checkBuffer[1] = byte;} 
    else if (byte == FLAG)
      *state = FLAG_RCV;
    else
      *state = Start;    
    break;
  case C_RCV:
    if (byte == BCC(checkBuffer[0],checkBuffer[1]))
      *state = BCC_OK;
    else if (byte == FLAG)
      *state = FLAG_RCV;
    else
      *state = Start;
    break;
  case BCC_OK:
    if (byte == FLAG)
      *state = DONE;
    else
      *state = Start;
    break;
  case DONE:
    break;
  }
}

int transmitter_SET(int fd){
  unsigned char buf[SET_SIZE] = {FLAG, A_ER, C_SET, BCC(A_ER, C_SET), FLAG};
  unsigned char buf_read[UA_SIZE]; // read buffer
  unsigned char checkBuf[2]; // checkBuf terá valores de A e C para verificar BCC
  int attempt = 0, res;
  volatile int STOP=FALSE;

  do{
    attempt++;
    res = write(fd,buf,SET_SIZE);
    if (res == -1) {
      log_error("transmitter: failed writing SET to buffer.");
      return -1;
    }
    
    log_message_number("Bytes sent to Receiver\n", res);
    
    alarm(linkLayer.timeout);
    failed = FALSE;

    state = Start;      
    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf_read,1);   /* returns after 1 char has been input */

      if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
        log_caution("transmitter: failed reading UA from receiver.");
        if (attempt < linkLayer.numTransmissions) {
          log_caution("Trying again...");
          failed = TRUE;
        }
        break;
      } else if (res == -1){
        log_error("transmitter: failed reading UA from buffer.");
        return -1;
      }
      
      log_message_number("Bytes read - ", res);
      log_hexa(buf_read[0]);
      
      stateMachine_SET_UA(&state, checkBuf, buf_read[0], TRANSMITTER);

      if (state == DONE || failed) STOP=TRUE;
    }
  }while (attempt < linkLayer.numTransmissions && failed);

  alarm(0); // cancel pending alarms

  if(failed) return -1;
  return fd;
}

int receiver_UA(int fd){
  unsigned char buf[SET_SIZE];
  unsigned char checkBuf[2]; // checkBuf terá valores de A e C para verificar BCC
  int res;
  volatile int STOP=FALSE;
  state = Start;

  while (STOP==FALSE) {       /* loop for input */
    res = read(fd,buf,1);   /* returns after 1 char has been input */
    if (res == -1) {
      log_error("receiver: failed reading SET from buffer.");
      return -1;
    }

    log_message_number("Bytes read - ", res);
    log_hexa(buf[0]);

    stateMachine_SET_UA(&state, checkBuf, buf[0], RECEIVER);
    if (state == DONE) STOP=TRUE;
  }

  unsigned char replyBuf[UA_SIZE] = {FLAG, A_ER, C_UA, BCC(A_ER, C_UA), FLAG};

  res = write(fd,replyBuf,UA_SIZE); //+1 para enviar o \0 
  if (res == -1) {
    log_error("receiver: failed writing UA to buffer.");
    return -1;
    }

  log_message_number("Bytes written\n", res); //res a contar com o \n e com o \0
  
  return fd;
}

int llopen(int porta, int type){
  int fd;
  char port[12]; // "/dev/ttyS11\0" <- 12 chars
  snprintf(port, 12, "/dev/ttyS%d", porta);
  
  if (initConnection(&fd, port) < 0){
    log_error("Error on initConnection");
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
    return transmitter_SET(fd);
    break;
  case RECEIVER:
    return receiver_UA(fd);
    break;
  default:
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
  unsigned char *dataBuffer = (unsigned char *)malloc(lenght * sizeof(unsigned char));
  unsigned char buf_read[255];
  volatile int STOP=FALSE;
  int attempt = 0;

  if (lenght > MAX_SIZE)
    return -1;

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
      dataBuffer = (unsigned char *) realloc(dataBuffer, currentLenght * sizeof(unsigned char)); 

      dataBuffer[k+1] = buffer[i] ^ 0x20;
      dataBuffer[k] = 0x7D;
      k++;
    }
    else{
      dataBuffer[k] = buffer[i];
    }
  }

  unsigned char finalBuffer[currentLenght + 6]; /*trama I completa*/
  int finalIndex = fillFinalBuffer(finalBuffer, buf1, buf2, dataBuffer, currentLenght);
  

  for (int i = 0; i<finalIndex; i++)
    log_hexa(finalBuffer[i]);

  /*sending trama I*/
  do{
    attempt++;
    res = write(fd,finalBuffer,sizeof(finalBuffer));
    if (res == -1) {
      log_error("writter: failed writing data to buffer.");
      return -1;
    }
    
    log_message_number("Bytes written \n", res);
    
    alarm(linkLayer.timeout);
    failed = FALSE;
    
    //state = Start;
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
        log_error("llwrite: failed reading RR from buffer.");
        return -1;
      }
      
      log_message_number("Bytes read ", res);
      log_hexa(buf_read[0]);

      //stateMachine_RR

      if (failed) STOP=TRUE;
    }
  }while (attempt < linkLayer.numTransmissions && failed);

  alarm(0);
  linkLayer.sequenceNumber ^= 1;

  return 0;
}

int stateMachine_Read(enum stateMachine *state, unsigned char *checkBuffer, char byte, unsigned char **buffer, int* buffersize){
  //printf("A:%#4.2x C:%#4.2x \n", checkBuffer[0], checkBuffer[1]);

  static int frameIndex;
  linkLayer.frame[frameIndex] = byte;
  switch (*state)
  {
  case Start:
  frameIndex = 0;
    if (byte == FLAG){
      *state = FLAG_RCV;
      frameIndex++;
    }  
    break;
  case FLAG_RCV:
    if (byte == A_ER) {
      *state = A_RCV;
      checkBuffer[0] = byte;
      frameIndex++;
    }
    else if (byte!= FLAG)
      *state = Start;
    break;
  case A_RCV:
    // caso mande a mensagem errada
    if (byte == C_I(linkLayer.sequenceNumber ^ 1)) return -1;
  
    if (byte == C_I(linkLayer.sequenceNumber)) {
      *state = C_RCV;
      checkBuffer[1] = byte;
      frameIndex++;
    } else if (byte == FLAG){
      *state = FLAG_RCV;
      frameIndex = 1;
    } else 
      *state = Start;
    break;
  case C_RCV:  
    if (byte == BCC(checkBuffer[0],checkBuffer[1])){
      *state = BCC_OK;
      frameIndex++;
    }
    else if (byte == FLAG){
      *state = FLAG_RCV;
      frameIndex = 1;
    }
    else
      return -1;
    break;
  case BCC_OK:
    frameIndex++;
    if (byte == FLAG){
      *buffer = (unsigned char *)malloc((frameIndex-6));
      *buffersize = 0;

      // Byte De-stuffing
      for (int i = 4; i < frameIndex - 2; i++)
      {
        if (linkLayer.frame[i] != 0x7D)
          (*buffer)[*buffersize] = linkLayer.frame[i];
        else{
          (*buffer)[*buffersize] = linkLayer.frame[i+1] ^ 0x20;
          i++;
        }
        (*buffersize)++;
      }
      *buffer = (unsigned char *) realloc(*buffer, (*buffersize)); 
      
      // Formação de BCC2
      unsigned char BCC2 = (*buffer)[0];
      for (int i = 1; i < *buffersize; i++)
        BCC2 = BCC2 ^ (*buffer)[i];

      if (linkLayer.frame[frameIndex-2]==BCC2){
        linkLayer.sequenceNumber = linkLayer.sequenceNumber ^ 1;
        *state = DONE;
      }
      else
        return -1;
    }
    break;
  case DONE:
    break;
  }

  return 0;
}

int llread(int fd, unsigned char *buffer){
  unsigned char buf[SET_SIZE];
  unsigned char checkBuf[2]; // checkBuf terá valores de A e C para verificar BCC
  unsigned char *dataBuf;
  int res, retBufferSize;
  volatile int STOP=FALSE;
  state = Start;

  unsigned char c;

  while (STOP==FALSE) {       
    res = read(fd,buf,1);   
    if (res == -1) {
      log_error("receiver: failed reading frame from buffer.");
      return -1;
    }

    log_message_number("Bytes read - ", res);
    log_hexa(buf[0]);
    
    if (stateMachine_Read(&state, checkBuf, buf[0], &dataBuf, &retBufferSize) == -1){
      c = C_REJ(linkLayer.sequenceNumber);
      break;
    }
    c = C_RR(linkLayer.sequenceNumber);
    if (state == DONE) STOP=TRUE;
  }
  unsigned char replyBuf[5] = {FLAG, A_ER, c, BCC(A_ER, c), FLAG};

  for (int i = 0; i < retBufferSize; i++){
    buffer[i] = dataBuf[i];
  }

  res = write(fd,replyBuf,5);
  if (res == -1) {
    log_error("receiver: failed writing response to buffer.");
    return -1;
  }

  linkLayer.sequenceNumber^=1;
  
  return retBufferSize; 
}

int llclose(int fd){
  // Falta Mandar Disc -> Disc -> UA

  if (tcsetattr(fd,TCSANOW,&oldtio) != 0){
    log_error("Error on tcsetattr");
    return -1;
  }
  if (close(fd) != 0){
    log_error("Error on tcsetattr");
    return -1;
  }
  log_success("Closing Successful");
  return 0;
}