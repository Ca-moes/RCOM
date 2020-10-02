/*Waiting for Input from Multiple Sources*/

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


int open_input_source(char *);
void handle_input_from_source(int); 
int MAX(int, int);

main()
{
     int    fd1, fd2;  /* input sources 1 and 2 */
     fd_set readfs;    /* file descriptor set */
     int    maxfd;     /* maximum file desciptor used */
     int    loop=1;    /* loop while TRUE */

     /* open_input_source opens a device, sets the port correctly, and
        returns a file descriptor */
     fd1 = open_input_source("/dev/ttyS1");   /* COM2 */
     if (fd1<0) exit(0);
     fd2 = open_input_source("/dev/ttyS2");   /* COM3 */
     if (fd2<0) exit(0);
     maxfd = max (fd1, fd2)+1;  /* maximum bit entry (fd) to test */

     /* loop for input */
     while (loop) {
       FD_SET(fd1, &readfs);  /* set testing for source 1 */
       FD_SET(fd2, &readfs);  /* set testing for source 2 */
       /* block until input becomes available */
       /* select will not load the CPU */
       select(maxfd, &readfs, NULL, NULL, NULL);
       if (FD_ISSET(fd1, &readfs))         /* input from source 1 available */
         handle_input_from_source(fd1);
       if (FD_ISSET(fd2, &readfs))         /* input from source 2 available */
         handle_input_from_source(fd2);
     }
    close(fd1);
    close(fd2);
}


int open_input_source(char * port)
{
    int fd = 0;
    
/* open the device to be non-blocking (read will return immediatly) */
    fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd <0) {
      perror(port); 
      return -1;
    }
    else
    return fd; 
}

void handle_input_from_source(int fd)
{
    int res = 0, i;
    char buf[255];
  
    res = read(fd,buf,255);
    buf[res]=0;
    for (i = 0; i < res; i++) printf("%c", buf[i]);
}

int max(int i1, int i2)
{
    if (i1 > i2)
      return i1;
    else
      return i2;
}