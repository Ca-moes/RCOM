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
#include <signal.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;
enum stateMachine state;

int failed = FALSE;

void atende() // atende alarme
{
  if (state != DONE)  failed = TRUE;
}

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
    if (byte == C_UA) {
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

    
    (void)signal(SIGALRM, atende);
    
    /* if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS10", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS11", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    } */


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
    newtio.c_cc[VMIN]     = 1;   /* blocking read until 5 chars received */



  /* 
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) pr�ximo(s) caracter(es)
  */



    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printf("New termios structure set\n");

    buf[0]= FLAG; // F
    buf[1]= A_ER; // A
    buf[2]= C_SET; // C
    buf[3]= BCC(A_ER, C_SET); // BCC
    buf[4]= FLAG; // F

    /* printf("conteudo de buf[0] : %c\n", buf[0]);
    printf("conteudo de buf[0] : %x\n", buf[0]);
    printf("conteudo de buf[0] : %#x\n", buf[0]);
    printf("conteudo de buf : %s\n", buf); */

    int attempt = 0;

    do{
      
      attempt++;
      res = write(fd,buf,SET_SIZE);   //envia o \0
      printf("%d bytes written\n", res);  

      alarm(3);
      failed = FALSE;

      state = Start;
      unsigned char checkBuf[2]; // checkBuf terá valores de A e C para verificar BCC

      while (STOP==FALSE) {       /* loop for input */
        res = read(fd,buf,1);   /* returns after 1 char has been input */

        printf("nº bytes lido: %d - ", res);
        printf("content: %#4.2x\n", buf[0]);

        determineState(&state, checkBuf, buf[0]);

        if (state == DONE || failed) STOP=TRUE;
      }

    }while (attempt < ATTEMPT_NUM && failed);


    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }



    close(fd);
    return 0;
}
