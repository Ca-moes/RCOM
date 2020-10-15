#ifndef PLA_HEADER
#define PLA_HEADER


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "macros.h"

#define BAUDRATE B38400

#define TRANSMITTER 1
#define RECEIVER 0

#define FALSE 0
#define TRUE 1

void initConnection(int *fd);

int llopen(int porta, int flag);

int llwrite(int fd, char *buffer, int lenght);

int llread(int fd, char *buffer);

int llclose(int fd);


#endif // PLA_HEADER