#ifndef CONNECT_HEADER
#define CONNECT_HEADER

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

#include "args.h"
FILE * socketFile;

int init(args args, int *socketfd);

int sendCommand(int socketfd, char * command);

int readResponse(int socketfd);

int readResponsePassive(int socketfd);

void parsePassive(char* line, char** ip, char** port);

#endif // CONNECT_HEADER
