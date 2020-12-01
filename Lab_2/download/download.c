#include "download.h"

int main(int argc, char** argv){ 
  if (argc != 2) {
    fprintf(stderr, "usage: download ftp://[<user>:<password>@]<host>/<url-path>\n");
    exit(1);
  }

  args arguments;
  int socketfd;
  char urlcpy[256];
  char command[256];

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
	readResponse(socketfd);
  
  sprintf(command, "user %s\r\n", arguments.user);
  sendCommand(socketfd, command);
  readResponse(socketfd);

  sprintf(command, "pass %s\r\n", arguments.password);
  sendCommand(socketfd, command);
	readResponse(socketfd);

  sprintf(command, "pasv\r\n");
  sendCommand(socketfd, command);
	readResponsePassive(socketfd);

  /* sprintf(command, "retr %s\r\n", arguments.path);
  sendCommand(socketfd, command);
	readResponse(socketfd); */

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

