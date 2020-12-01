#ifndef ARGS_HEADER
#define ARGS_HEADER

#include <stdio.h>
#include <string.h>

typedef struct args
{
  char* user;
  char* password;
  char* host;
  char* path;
} args;


int parseArgs(char *url, args * args);

#endif // ARGS_HEADER
