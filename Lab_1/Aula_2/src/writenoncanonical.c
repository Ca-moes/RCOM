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
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd, res;
    struct termios oldtio,newtio;
    char buf[255];
    int i;
    
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


    res = write(fd,buf,SET_SIZE);   //envia o \0
    printf("%d bytes written\n", res);  



    unsigned char replybuffer[255];
    i=0;

    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf,1);   /* returns after 1 char has been input */
      buf[res]=0;               /* so we can printf... */

      printf("nº bytes lido: %d - ", res);
      printf("content: %s\n", buf);

      replybuffer[i] = buf[0];
      i++;

      if (buf[res-1]=='\0') STOP=TRUE;
    }

    printf("Message received: %s\n",replybuffer);





    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    close(fd);
    return 0;
}
