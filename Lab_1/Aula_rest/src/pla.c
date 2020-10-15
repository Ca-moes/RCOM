// Protocolo de Ligação de Dados
#include "pla.h"

struct termios oldtio; // utilizado para fechar a ligação em llclose
enum stateMachine state;
int failed = FALSE;

void initConnection(int *fd, char *port){
  struct termios newtio;

  *fd = open(port, O_RDWR | O_NOCTTY );
  if (fd < 0) {perror(port); exit(-1); }

  if ( tcgetattr(*fd,&oldtio) == -1) { /* save current port settings */
    perror("tcgetattr");
    exit(-1);
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
  printf("New termios structure set\n");
}

void atende(){ 
  if (state != DONE){ 
    printf("receiver didn't answer\n");
    failed = TRUE;
    return;
  }
}

void stateMachine_SET_UA(enum stateMachine *state, unsigned char *checkBuffer, char byte, int type){
  // TO-DO máquina de estados
  printf("A:%#4.2x C:%#4.2x \n", checkBuffer[0], checkBuffer[1]);
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

void transmitter_SET(int fd){
  unsigned char buf[SET_SIZE] = {FLAG, A_ER, C_SET, BCC(A_ER, C_SET), FLAG};
  unsigned char buf_read[UA_SIZE]; // read buffer
  unsigned char checkBuf[2]; // checkBuf terá valores de A e C para verificar BCC
  int attempt = 0, res;
  volatile int STOP=FALSE;

  do{
    attempt++;
    res = write(fd,buf,SET_SIZE);
    log_message_number("Bytes written ", res);
    
    alarm(3);
    failed = FALSE;

    state = Start;      
    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf_read,1);   /* returns after 1 char has been input */
      if (res == -1) break;
      
      log_message_number("Bytes read ", res);
      log_hexa(buf_read[0]);
      
      stateMachine_SET_UA(&state, checkBuf, buf_read[0], TRANSMITTER);
      if (state == DONE || failed) STOP=TRUE;
    }
  }while (attempt < ATTEMPT_NUM && failed);
}

void receiver_UA(int fd){
  unsigned char buf[SET_SIZE];
  unsigned char checkBuf[2]; // checkBuf terá valores de A e C para verificar BCC
  int res;
  volatile int STOP=FALSE;
  state = Start;

  while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf,1);   /* returns after 1 char has been input */

      printf("nº bytes lido: %d - ", res);
      printf("content: %#4.2x\n", buf[0]);

      stateMachine_SET_UA(&state, checkBuf, buf[0], RECEIVER);
      if (state == DONE) STOP=TRUE;
    }

    unsigned char replyBuf[UA_SIZE] = {FLAG, A_ER, C_UA, BCC(A_ER, C_UA), FLAG};

    res = write(fd,replyBuf,UA_SIZE); //+1 para enviar o \0 
    printf("%d bytes written\n", res); //res a contar com o \n e com o \0
}


int llopen(int porta, int type){
  int fd;
  char port[12]; // "/dev/ttyS11\0" <- 12 chars
  snprintf(port, 12, "/dev/ttyS%d", porta);
  
  initConnection(&fd, port);
  
  // Set Handler for Alarm
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = atende;
  sa.sa_flags = 0;
  sigaction(SIGALRM, &sa, NULL);

  switch (type)
  {
  case TRANSMITTER:
    transmitter_SET(fd);
    break;
  case RECEIVER:
    receiver_UA(fd);
    break;
  default:
    return -1;
    break;
  }

  return fd;
}

int llwrite(int fd, char *buffer, int lenght){
  return 0;
}

int llread(int fd, char *buffer){
  return 0;
}

int llclose(int fd){
  // Falta Mandar Disc -> Disc -> UA

  tcsetattr(fd,TCSANOW,&oldtio);
  close(fd);
  return 0;
}