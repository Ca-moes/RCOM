/*Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    char buf[255];
    int i;
    
    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tserial SerialPort\n\tex: serial /dev/ttyS1\n");
      exit(1);
    }
    
  /*
    Open serial port device for reading and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */
   fd = open(argv[1], O_RDONLY | O_NOCTTY );
   if (fd <0) {perror(argv[1]); exit(-1); }

   tcgetattr(fd,&oldtio); /* save current serial port settings */
   bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

  /*
    BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
    CRTSCTS : output hardware flow control
              ATTENTION:  only used if the cable has all necessary lines (is 
              a null modem cable), otherwise the output flow control is set 
              automatically by software by the driver. 
    CS8     : 8n1 (8bit,no parity,1 stopbit)
    CLOCAL  : local connection, no modem control
    CREAD   : enable receiving characters
  */
   newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;

  /*
    IGNPAR  : ignore framing and parity errors
    ICRNL   : map CR to NL (otherwise a CR input on the other computer
              will not terminate input)
    otherwise make device raw (no other input processing)
  */
   newtio.c_iflag = IGNPAR | ICRNL;

  /*
   Raw output.
  */
   newtio.c_oflag = 0;

  /*
    ICANON  : enable canonical input
    disable all echo functionality, and don't send signals to calling program
  */
   newtio.c_lflag = ICANON;

  /*
    initialize all control characters
    default values can be found in /usr/include/termios.h, and are given
    in the comments, but we don't need them here
  */
   newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
   newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
   newtio.c_cc[VERASE]   = 0;     /* del */
   newtio.c_cc[VKILL]    = 0;     /* @ */
   newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
   newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
   newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
   newtio.c_cc[VSWTC]    = 0;     /* '\0' */
   newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
   newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
   newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
   newtio.c_cc[VEOL]     = 0;     /* '\0' */
   newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
   newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
   newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
   newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
   newtio.c_cc[VEOL2]    = 0;     /* '\0' */

  /*
    now clean the line and activate the settings for the port
  */
   /* tcflush() discards data written to the object referred  to */
   /* by  fd but not transmitted, or data received but not read, */
   /* depending on the value of queue_selector:                  */
   /*   TCIFLUSH - flushes data received but not read.           */
   tcflush(fd, TCIFLUSH);
   tcsetattr(fd,TCSANOW,&newtio);

  /*
    terminal settings done, now handle input
    In this example, inputting a 'z' at the beginning of a line will
    exit the program.
  */
  
  
   while (STOP==FALSE) {     /* loop until we have a terminating condition */
   /* read blocks program execution until a line terminating character is
      input, even if more than 255 chars are input. If the number
      of characters read is smaller than the number of chars available,
      subsequent reads will return the remaining chars. res will be set
      to the actual number of characters actually read */
      
      res = read(fd,buf,255);
      buf[res]='\0';             /* set end of string, so we can printf */
      for (i = 0; i < res; i++) printf("%c", buf[i]);
      printf("\n");
      if (buf[0] == 'z') STOP=TRUE;
   }

   /* restore the old port settings */
   tcsetattr(fd,TCSANOW,&oldtio);
   close(fd);
   return 0;
}
