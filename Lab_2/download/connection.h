#ifndef CONNECT_HEADER
#define CONNECT_HEADER

#include <sys/socket.h>
#include <netinet/in.h>

#include "args.h"

int init(args args, int *socketfd);

#endif // CONNECT_HEADER
