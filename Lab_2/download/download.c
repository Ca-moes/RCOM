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
	readResponse(socketfd);
  
  sprintf(command, "user %s\r\n", arguments.user);
  sendCommand(socketfd, command);
  readResponse(socketfd);

  sprintf(command, "pass %s\r\n", arguments.password);
  sendCommand(socketfd, command);
	readResponse(socketfd);

  char* ip; ip = malloc(16);
  int port;

  sprintf(command, "pasv\r\n");
  sendCommand(socketfd, command);
	readResponsePassive(socketfd, &ip, &port);
  
  puts(ip);
  printf("port: %d\n", port);

  if (init(ip, port, &socketfd_rec) != 0){
    printf("Error: init()\n");
    return -1;
  }

  sprintf(command, "retr %s\r\n", arguments.path);
  sendCommand(socketfd, command);
	/* readResponse(socketfd); */

/*   saveFile(socketfd_rec);
 */
  return 0;
}
