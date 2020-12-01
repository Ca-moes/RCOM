#include "args.h"

int parseArgs(char *url, args *args){
  puts(url);

  char* ftp = strtok(url, "/");       // ftp:
  char* urlrest = strtok(NULL, "/");  // [<user>:<password>@]<host>
  char* path = strtok(NULL, "");      // <url-path>

  if (strcmp(ftp, "ftp:") != 0)
    return 1;
  
  char* user = strtok(urlrest, ":");
  char* pass = strtok(NULL, "@");

  // no user:password given
  if (pass == NULL)
  {
    user = "user";
    pass = "pass";
    args->host = urlrest;
  } else
    args->host = strtok(NULL, "");
  
  args->path = path;
  args->user = user;
  args->password = pass;
  
  return 0;
}