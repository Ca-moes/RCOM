#include "download.h"

int main(int argc, char** argv){ 
  if (argc != 2) {
    fprintf(stderr, "usage: download ftp://[<user>:<password>@]<host>/<url-path>\n");
    exit(1);
  }

  args arguments;
  int socketfd, socketfd_rec;
  char urlcpy[256];
  char command[256];

  strcpy(urlcpy, argv[1]);

  if (parseArgs(urlcpy, &arguments) != 0){
    printf("usage: %s ftp://[<user>:<password>@]<host>/<url-path>\n",argv[0]); 
		return -1;
  }

  printf("\nhost: %s\npath: %s\nuser: %s\npassword: %s\nfile name: %s\nhost name: %s\nip address: %s\n\n", 
  arguments.host, arguments.path, arguments.user, arguments.password, arguments.file_name, arguments.host_name, arguments.ip);
  
  if (init(arguments.ip, 21, &socketfd) != 0){
    printf("Error: init()\n");
    return -1;
  }
	readResponse();
  
  // login
  sprintf(command, "user %s\r\n", arguments.user);
  sendCommand(socketfd, command);
  readResponse();
  sprintf(command, "pass %s\r\n", arguments.password);
  sendCommand(socketfd, command);
	readResponse();

  // get ip and port
  char* ip; ip = malloc(16);
  int port;
  sprintf(command, "pasv\r\n");
  sendCommand(socketfd, command);
	readResponsePassive(&ip, &port);
  printf("ip: %s\nport: %d\n", ip, port);

  // init new socket to read file
  if (init(ip, port, &socketfd_rec) != 0){
    printf("Error: init()\n");
    return -1;
  }

  sprintf(command, "retr %s\r\n", arguments.path);
  sendCommand(socketfd, command);

  char * buf;
	size_t bytesRead = 0;
  for (int i = 0; i < 5; i++)
  {
    getline(&buf, &bytesRead, socketFile);
    printf("buf: %s", buf);
  }
  
  printf("------------\n");

  FILE * socket_recFile = fdopen(socketfd_rec, "r");
  for (int i = 0; i < 5; i++)
  {
    getline(&buf, &bytesRead, socket_recFile);
    printf("buf: %s", buf);
  }

	/* readResponse(socketfd);
  saveFile(socketfd_rec);  */

  return 0;
}
