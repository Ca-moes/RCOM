#include "download.h"

/**
 * @brief Main function called to execute the download
 * 
 * @param argc number of arguments
 * @param argv value of the arguments
 * @return int return value
 */
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
  
  // Ao fazer init o valor do file_name fica lixo, vá-se lá saber porque
  // Então guarda-se o valor do file name na main
  char fileName[64];
  strcpy(fileName, arguments.file_name);

  char* tempip; tempip = malloc(16);
  strcpy(tempip, arguments.ip);

  if (init(tempip, 21, &socketfd) != 0){
    printf("Error: init()\n");
    return -1;
  }
  socketFile = fdopen(socketfd, "r");
	readResponse();

  // login
  sprintf(command, "user %s\r\n", arguments.user);
  sendCommand(socketfd, command);
  readResponse();
  sprintf(command, "pass %s\r\n", arguments.password);
  sendCommand(socketfd, command);
	readResponse();

  // get ip and port
  char* ip; ip = malloc(17);
  int port;
  sprintf(command, "pasv\r\n");
  sendCommand(socketfd, command);
	readResponsePassive(&ip, &port);
  printf("ip: %s\nport: %d\n", ip, port);

  if (init(ip, port, &socketfd_rec) != 0){
    printf("Error: init()\n");
    return -1;
  }

  sprintf(command, "retr %s\r\n", arguments.path);
  sendCommand(socketfd, command);
  readResponse();

  saveFile(fileName, socketfd_rec);
  return 0;
}
