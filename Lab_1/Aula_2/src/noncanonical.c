/*Non-Canonical Input Processing*/
#include "macros.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

void determineState(enum stateMachine *state, unsigned char *checkBuffer, char byte){
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
  case A_RCV:
    if (byte == C_SET) {
      *state = C_RCV;
      checkBuffer[1] = byte;
    }
    else if (byte == FLAG){
      *state = FLAG_RCV;
    }
    else{
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

int main(int argc, char** argv)
{
    int fd, res;
    struct termios oldtio,newtio;
    unsigned char buf[255];

  /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */
  
    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
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
    newtio.c_cc[VMIN]     = 1;   /* blocking read until 1 chars received */




    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printf("New termios structure set\n");


    unsigned char replyBuf[255], checkBuf[2]; // checkBuf terá valores de A e C para verificar BCC
    enum stateMachine state = Start;


    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf,1);   /* returns after 1 char has been input */

      printf("nº bytes lido: %d - ", res);
      printf("content: %#4.2x\n", buf[0]);

      determineState(&state, checkBuf, buf[0]);

      if (state == DONE) STOP=TRUE;
    }



    replyBuf[0]= FLAG; // F
    replyBuf[1]= A_ER; // A
    replyBuf[2]= C_UA; // C
    replyBuf[3]= BCC(A_ER, C_UA); // BCC
    replyBuf[4]= FLAG; // F

    res = write(fd,replyBuf,UA_SIZE); //+1 para enviar o \0 
    printf("%d bytes written\n", res); //res a contar com o \n e com o \0


    tcsetattr(fd,TCSANOW,&oldtio);
    close(fd);
    return 0;
}

