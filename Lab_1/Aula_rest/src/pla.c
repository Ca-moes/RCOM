// Protocolo de Ligação de Dados
#include "pla.h"

struct termios oldtio; // utilizado para fechar a ligação

void initConnection(int *fd){
  struct termios newtio;

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

int llopen(int porta, int type){
  int fd;
  char port[12]; // "/dev/ttyS11\0" <- 12 chars

  snprintf(port, 12, "/dev/ttyS%d", porta);
  fd = open(port, O_RDWR | O_NOCTTY );
  if (fd <0) {perror(port); exit(-1); }

  initConnection(&fd);
  
  switch (type)
  {
  case TRANSMITTER:
    /* code */
    break;
  case RECEIVER:
    /* code */
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