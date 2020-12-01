#include "download.h"

int main(int argc, char** argv){ 
  args arguments;
  int socketfd;

  if (parseArgs(argv[1], &arguments) != 0){
    printf("usage: %s ftp://[<user>:<password>@]<host>/<url-path>\n",argv[0]); 
		return -1;
  }

  printf("\nhost: %s\npath: %s\nuser: %s\npassword: %s\nfile name: %s\nhost name: %s\nip address: %s\n", 
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
  
  int sent = write(socketfd, "user anonymous\r\n", strlen("user anonymous\r\n"));
  printf("sent: %d\n", sent);
  printf("len: %ld\n", strlen("user 1234"));

  while (1){
    getline(&buf, &bytesRead, socket);
    printf("buf: %s", buf);
    if (buf[3] == ' '){
      break;
    }
  }

  sent = write(socketfd, "pass 1234\r\n", strlen("pass 1234\r\n"));
  printf("sent: %d\n", sent);
  printf("len: %ld\n", strlen("pass 1234"));

  while (1){
    getline(&buf, &bytesRead, socket);
    printf("buf: %s", buf);
    if (buf[3] == ' '){
      break;
    }
  }

  sent = write(socketfd, "pasv\r\n", strlen("pasv\r\n"));
  printf("sent: %d\n", sent);
  printf("len: %ld\n", strlen("pasv\r\n"));

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

