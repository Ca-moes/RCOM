/*Asynchronous Input*/


#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

void signal_handler_IO (int status);   /* definition of signal handler */
int wait_flag=TRUE;                    /* TRUE while no signal received */

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    struct sigaction saio;           /* definition of signal action */
    char buf[255];
    int i;

    //Uses either COM1 or COM2     
  	if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tasync_read SerialPort\n\tex: async_read /dev/ttyS1\n");
      exit(1);
    }

    /* open the device to be non-blocking (read will return immediatly) */
    fd = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK);
//    fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd <0) {perror(argv[1]); exit(-1); }


    /* To receive the SIGIO we need to perform three steps: */
    /* 1. establish a signal handler for the signal - sigaction() */
    /* 2. set the pid or gid to receive the signal for the descriptor */
    /* 3. enable asynchronous I/O on the descriptor*/

    /* install the signal handler before making the device asynchronous */
    saio.sa_handler = signal_handler_IO;/*signal handler address or SI_IGN ou SIG_DFL*/
    saio.sa_flags = 0;          /*gives  a  mask of signals which should */
                                /*be blocked during execution of the */
                                /*signal handler.  In addition,  the signal*/
                                /*which triggered the handler will be blocked,*/
                                /*unless the SA_NODEFER or SA_NOMASK flags */
                                /*are used.*/
    saio.sa_restorer = NULL;    /*obsolete*/
    sigaction(SIGIO,&saio,NULL);

    /* allow the process to receive SIGIO */
    /* set the process ID or process group ID to receive */
    /* the SIGIO signal. A positive arg specifies the pid. */
    /* A negative arg implies a process group ID equal to */
    /* absolute value of arg*/
    fcntl(fd, F_SETOWN, getpid());
    /* Make the file descriptor asynchronous (the manual page says only
       O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
    fcntl(fd, F_SETFL, FASYNC);

    tcgetattr(fd,&oldtio); /* save current port settings */
    /* set new port settings for canonical input processing */
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | ICRNL;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;
    newtio.c_cc[VMIN]=1;
    newtio.c_cc[VTIME]=0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);

    /* loop while waiting for input. normally we would do something
       useful here */
    while (STOP==FALSE) {
      write(1, ".", 1);usleep(100000);
      /* after receiving SIGIO, wait_flag = FALSE, input is available
         and can be read */
      if (wait_flag==FALSE) {
        res = read(fd,buf,255);
        buf[res]=0;
        for (i = 0; i < res; i++) printf("%c", buf[i]);
        if (res==1) STOP=TRUE; /* stop loop if only a CR was input */
        wait_flag = TRUE;      /* wait for new input */
      }
    }
    /* restore old port settings */
    tcsetattr(fd,TCSANOW,&oldtio);
    close(fd);
}

/***************************************************************************
* signal handler. sets wait_flag to FALSE, to indicate above loop that     *
* characters have been received.                                           *
***************************************************************************/

void signal_handler_IO (int status)
{
    printf("received SIGIO signal");
    wait_flag = FALSE;
}

