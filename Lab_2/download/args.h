#ifndef ARGS_HEADER
#define ARGS_HEADER

#define h_addr h_addr_list[0]	//The first address in h_addr_list. 

#include <stdio.h>
#include <string.h>
#include <netdb.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef struct args
{
  char* user;
  char* password;
  char* host;
  char* path;
  char* file_name;
  char* host_name;
  char* ip;

} args;


int parseArgs(char *url, args * args);

int getIp(char *host, args * args);

int getFileName(args * args);
#endif // ARGS_HEADER
