#include "args.h"

int parseArgs(char *url, args *args){
  char* ftp = strtok(url, "/");       // ftp:
  char* urlrest = strtok(NULL, "/");  // [<user>:<password>@]<host>
  char* path = strtok(NULL, "");      // <url-path>

  if (strcmp(ftp, "ftp:") != 0){
    printf("Error: Not using ftp\n");
    return 1;
  }
  
  char* user = strtok(urlrest, ":");
  char* pass = strtok(NULL, "@");


  // no user:password given
  if (pass == NULL)
  {
    user = "anonymous";
    pass = "pass";
    args->host = urlrest;
  } else
    args->host = strtok(NULL, "");
  

  args->path = path;
  args->user = user;
  args->password = pass;
  
  if (getIp(args->host, args) != 0){
    printf("Error: getIp()\n");
    return 2;
  }

  if (getFileName(args) != 0){
    printf("Error: getFileName()\n");
    return 3;
  }
  
  return 0;
}

int getIp(char *host, args *args){
  struct hostent *h;

  if ((h=gethostbyname(host)) == NULL) {  
    herror("gethostbyname");
    return 1;
  }

  args->host_name = h->h_name;
  args->ip = inet_ntoa( *( (struct in_addr *)h->h_addr ) );
  return 0;
}

int getFileName(args * args){
  char fullpath[256];
  strcpy(fullpath, args->path);
  char* token = strtok(fullpath, "/");
  while( token != NULL ) {
    args->file_name = token;
    token = strtok(NULL, "/");
  }
  return 0;
}
