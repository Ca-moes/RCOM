/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

int main(int argc, char** argv)
{

 
    printf("\n");

    system("socat -d -d PTY,link=/dev/ttyS10,mode=777 PTY,link=/dev/emulatorTx,mode=777 &");
    sleep(1);
    printf("\n");

    system("socat -d -d PTY,link=/dev/ttyS11,mode=777 PTY,link=/dev/emulatorRx,mode=777 &");
    sleep(1);


    printf( "\n \n"
            "Transmitter must open /dev/ttyS10 \n"
            "Receiver must open /dev/ttyS11 \n \n"
            "The cable program is sensible to the following interative commands:\n"
            "--- on   : connects the cable and data is exchanged (default state)\n"
            "--- off  : disconnects the cable disabling data to be exchanged\n"
            "--- end  : terminates de program \n \n" );

    int fdTx;
    struct termios oldtioTx,newtioTx;

    fdTx = open("/dev/emulatorTx", O_RDWR | O_NOCTTY );
    if (fdTx <0) {perror("/dev/emulatorTx"); exit(-1); }

    if ( tcgetattr(fdTx,&oldtioTx) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtioTx, sizeof(newtioTx));
    newtioTx.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtioTx.c_iflag = IGNPAR;
    newtioTx.c_oflag = 0;
    newtioTx.c_lflag = 0;
    newtioTx.c_cc[VTIME]    = 1;   /* inter-character timer unused */
    newtioTx.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */
    tcflush(fdTx, TCIOFLUSH);
    if ( tcsetattr(fdTx,TCSANOW,&newtioTx) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printf("cable\n");

    int fdRx;
    struct termios oldtioRx,newtioRx;
    fdRx = open("/dev/emulatorRx", O_RDWR | O_NOCTTY );
    if (fdRx <0) {perror("/dev/emulatorRx"); exit(-1); }

    if ( tcgetattr(fdRx,&oldtioRx) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtioRx, sizeof(newtioRx));
    newtioRx.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtioRx.c_iflag = IGNPAR;
    newtioRx.c_oflag = 0;
    newtioRx.c_lflag = 0;
    newtioRx.c_cc[VTIME]    = 1;   /* inter-character timer unused */
    newtioRx.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */
    tcflush(fdRx, TCIOFLUSH);
    if ( tcsetattr(fdRx,TCSANOW,&newtioRx) == -1) {
      perror("tcsetattr");
      exit(-1);
    }



int oldf;
oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);


unsigned char tx2rx[512];
unsigned char rx2tx[512];
unsigned char rxStdin[512];

int fromTx, toRx;
int fromRx, toTx;
int fromStdin;
int connection=100;

    while (STOP==FALSE) {    

      fromTx = read(fdTx,tx2rx,512); 
      if(fromTx){ 
          if(connection){
             toRx=write(fdRx,tx2rx,fromTx);
             printf("fromTx=%d > toRx= %d \n", fromTx, toRx);
             fromTx=0;
             toRx=0;
             } else {
                 printf("fromTx=%d > toRx= CONNECTION OFF \n", fromTx);
                 fromTx=0;
                 toRx=0;
                }
          }


      fromRx = read(fdRx,rx2tx,512); 
      if(fromRx){ 
          if(connection){
              toTx=write(fdTx,rx2tx,fromRx);
              printf("toTx= %d < fromRx=%d \n", toTx, fromRx);
              fromRx=0;
              toTx=0;
              } else {
                  printf("toTx= CONNECTION OFF < fromRx=%d \n",  fromRx);
                  fromRx=0;
                  toTx=0;
                  }
          }


      fromStdin=read(STDIN_FILENO, rxStdin, 512);
      if(fromStdin>0){
          rxStdin[fromStdin-1]=0;
          if (strcmp(rxStdin, "off")==0 ) {
              connection=0;
              printf("CONNECTION OFF\n");
              }
          if (strcmp(rxStdin, "on")==0 ) {
              connection=100;
              printf("CONNECTION ON\n");
              }
          if (strcmp(rxStdin, "end")==0 ) {
              printf("END OF THE PROGRAM\n");
              STOP=TRUE;
              }
          }

    }



if( tcsetattr(fdRx,TCSANOW,&oldtioRx)==-1){;
      perror("tcsetattr");
      exit(-1);
      }


if( tcsetattr(fdTx,TCSANOW,&oldtioTx)==-1){;
      perror("tcsetattr");
      exit(-1);
      }


//fclose(fdTx);
//fclose(fdRx);
//fclose(STDIN_FILENO);

system("killall socat");
return 0;
}
