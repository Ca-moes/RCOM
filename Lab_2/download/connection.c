#include "connection.h"

int init(args args, int *socketfd){
  struct	sockaddr_in server_addr;

  /*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(args.ip);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(21);		/*server TCP port must be network byte ordered */
    
	/*opens a TCP socket*/
	if ((*socketfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    perror("socket()");
    return 1;
  }

	/*connects to the server*/
  if(connect(*socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    perror("connect()");
		return 1;
	}

  socketFile = fdopen(*socketfd, "r");

  return 0;
}

/*
todo falta verificar se, caso não mande tudo de uma vez, mande o resto
*/
int sendCommand(int socketfd, char * command){
  printf(" about to send command: \n> %s", command);
  int sent = send(socketfd, command, strlen(command), 0);
  if (sent == 0){
    printf("sendCommand: Connection closed");
    return 1;
  }
  if (sent == -1){
    printf("sendCommand: error");
    return 2;
  }
  printf(" command sent\n");
  return 0;
}

int readResponse(int socketfd){
  char * buf;
	size_t bytesRead = 0;

  while (1){
    getline(&buf, &bytesRead, socketFile);
    printf("buf: %s", buf);
    if (buf[3] == ' '){
      break;
    }
  }

  return 0;
}

int readResponsePassive(int socketfd){
  char * buf;
	size_t bytesRead = 0;

  while (1){
    getline(&buf, &bytesRead, socketFile);
    printf("buf: %s", buf);
    if (buf[3] == ' '){
      break;
    }
  }

  char ip[16];
  char port[8];
  parsePassive(buf, ip, port);
  
  puts(ip);
  puts(port);


  return 0;
}

void parsePassive(char* line, char** ip, char** port){
  printf("parse ip and port\n");
  char tempip[16];
  char tempport[8];
  
  strtok(line, "(");       // ftp:
  char* ip1 = strtok(NULL, ",");       // ftp:
  puts(ip1);
  char* ip2 = strtok(NULL, ",");       // ftp:
  puts(ip2);
  char* ip3 = strtok(NULL, ",");       // ftp:
  puts(ip3);
  char* ip4 = strtok(NULL, ",");       // ftp:
  puts(ip4);

  sprintf(ip, "%s.%s.%s.%s", ip1, ip2, ip3, ip4);
  puts(ip);

  char* p1 = strtok(NULL, ",");       // ftp:
  puts(p1);
  char* p2 = strtok(NULL, ")");       // ftp:
  puts(p2);


  sprintf(port, "%d", atoi(p1)*256 + atoi(p2));
  puts(port);
}