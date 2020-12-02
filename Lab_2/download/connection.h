#ifndef CONNECT_HEADER
#define CONNECT_HEADER

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "args.h"
FILE * socketFile;

int init(char *ip, int port, int *socketfd);

int sendCommand(int socketfd, char * command);

int readResponse();

int readResponsePassive(char** ip, int *port);

void parsePassive(char* line, char** ip, int *port);

int saveFile(char* filename, int socketfd);
#endif // CONNECT_HEADER
