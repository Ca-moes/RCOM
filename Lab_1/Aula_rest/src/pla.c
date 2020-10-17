// Protocolo de Ligação de Dados
#include "pla.h"

struct termios oldtio; // utilizado para fechar a ligação em llclose
enum stateMachine state;
int failed = FALSE;

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
  //printf("A:%#4.2x C:%#4.2x \n", checkBuffer[0], checkBuffer[1]);
  switch (*state)
  {
  case Start:
    if (byte == FLAG) *state = FLAG_RCV;    
    break;
  case FLAG_RCV:
    if (byte == A_ER) {
      *state = A_RCV;
      checkBuffer[0] = byte;
    }
    else if (byte!= FLAG){
      *state = Start;
    }
    break;
  case A_RCV:;
    // ; de cima aqui explicado -> https://stackoverflow.com/a/19830820
    int C_byte;
    if (type == TRANSMITTER) C_byte = C_UA;
    else if (type == RECEIVER) C_byte = C_SET;

    if (byte == C_byte) {
      *state = C_RCV;
      checkBuffer[1] = byte;
      } else if (byte == FLAG){
        *state = FLAG_RCV;
      } else{
        *state = Start;
      }
    
    break;
  case C_RCV:
    if (byte == BCC(checkBuffer[0],checkBuffer[1])){
      *state = BCC_OK;
    }
    else if (byte == FLAG){
      *state = FLAG_RCV;
    }
    else{
      *state = Start;
    }
    // precisa de valores de A & C
    break;
  case BCC_OK:
    if (byte == FLAG){
      *state = DONE;
    }
    else{
      *state = Start;
    }
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
    
    alarm(3);
    failed = FALSE;

    state = Start;      
    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf_read,1);   /* returns after 1 char has been input */
      
      if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
        log_caution("transmitter: failed reading UA from receiver.");
        if (attempt < ATTEMPT_NUM) log_caution("Trying again...");
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
  }while (attempt < ATTEMPT_NUM && failed);

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

int llwrite(int fd, char *buffer, int lenght){
  return 0;
}

int llread(int fd, char *buffer){
  return 0;
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