#include "download.h"

int main(int argc, char** argv){ 
  if (argc != 2) {
    fprintf(stderr, "usage: download ftp://[<user>:<password>@]<host>/<url-path>\n");
    exit(1);
  }

  args arguments;
  int socketfd;
  char urlcpy[256];

  strcpy(urlcpy, argv[1]);

  if (parseArgs(urlcpy, &arguments) != 0){
    printf("usage: %s ftp://[<user>:<password>@]<host>/<url-path>\n",argv[0]); 
		return -1;
  }

  printf("\nhost: %s\npath: %s\nuser: %s\npassword: %s\nfile name: %s\nhost name: %s\nip address: %s\n\n", 
  arguments.host, arguments.path, arguments.user, arguments.password, arguments.file_name, arguments.host_name, arguments.ip);
  
  if (init(arguments, &socketfd) != 0){
    printf("Error: init()\n");
    return -1;
  }
  

 	FILE * socket = fdopen(socketfd, "r");
	char * buf;
	size_t bytesRead = 0;

  while (1){
    getline(&buf, &bytesRead, socket);
    printf("buf: %s", buf);
    if (buf[3] == ' '){
      break;
    }
  }
  
  char command[256];
  sprintf(command, "user %s\r\n", arguments.user);
  printf("command: %s", command);
  int sent = send(socketfd, command, strlen(command), 0);
  printf("sent: %d\n", sent);
  printf("len: %ld\n", strlen(command));

  while (1){
    getline(&buf, &bytesRead, socket);
    printf("buf: %s", buf);
    if (buf[3] == ' '){
      break;
    }
  }

  sprintf(command, "pass %s\r\n", arguments.password);
  printf("command: %s", command);
  sent = send(socketfd, command, strlen(command), 0);
  printf("sent: %d\n", sent);
  printf("len: %ld\n", strlen(command));

  while (1){
    getline(&buf, &bytesRead, socket);
    printf("buf: %s", buf);
    if (buf[3] == ' '){
      break;
    }
  }

  sprintf(command, "pasv\r\n");
  printf("command: %s", command);
  sent = send(socketfd, command, strlen(command), 0);
  printf("sent: %d\n", sent);
  printf("len: %ld\n", strlen(command));

  while (1){
    getline(&buf, &bytesRead, socket);
    printf("buf: %s", buf);
    if (buf[3] == ' '){
      break;
    }
  }

  return 0;
}

/* não funfa assim
char* command = "user anonymous";
  int len = strlen(command);
  int bytes_sent = send(socketfd, command, len, 0);

  printf("bytes_sent: %d\n", bytes_sent);

  if (bytes_sent < 0)
  {
    int errorInt = xn_getlasterror();
    const char* errorString = xn_geterror_string(errorInt);
    printf("error : %s", errorString);
  }
  

  char buf[1024];
  int received = recv(socketfd, buf, 1024, 0);

  printf("received: %d\n", received);
*/

