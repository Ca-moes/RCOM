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

int llopen(int porta, int flag){
  int fd;
  char port[12]; // "/dev/ttyS11\0" <- 12 chars

  printf(" porta : %d\n", porta);
  snprintf(port, 12, "/dev/ttyS%d", porta);
  printf(" port : %s\n", port);

  fd = open(port, O_RDWR | O_NOCTTY );
  if (fd <0) {perror(port); exit(-1); }

  initConnection(&fd);
  




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