#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#define h_addr h_addr_list[0]

char* getHostIp(char * url);

int connect_ftp(char * url,int porta);
