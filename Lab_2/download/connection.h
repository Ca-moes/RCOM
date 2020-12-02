#ifndef CONNECT_HEADER
#define CONNECT_HEADER

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#include "args.h"
FILE * socketFile;
FILE * socket_recFile;

int init(char *ip, int port, int *socketfd);

int sendCommand(int socketfd, char * command);

int readResponse();

int readResponsePassive(char** ip, int *port);

void parsePassive(char* line, char** ip, int *port);

void saveFile(int socketfd);
#endif // CONNECT_HEADER
