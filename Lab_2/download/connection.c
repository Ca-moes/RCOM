#include "connection.h"

int init(char *ip, int port, int *socketfd){
  struct	sockaddr_in server_addr;

  /*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(port);		/*server TCP port must be network byte ordered */
    
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

int readResponse(){
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

int readResponsePassive(char** ip, int *port){
  char * buf;
	size_t bytesRead = 0;

  while (1){
    getline(&buf, &bytesRead, socketFile);
    printf("buf: %s", buf);
    if (buf[3] == ' '){
      break;
    }
  }

  parsePassive(buf, ip, port);
  
  return 0;
}

void parsePassive(char* line, char** ip, int *port){  
  strtok(line, "(");       
  char* ip1 = strtok(NULL, ",");       // 193
  char* ip2 = strtok(NULL, ",");       // 137
  char* ip3 = strtok(NULL, ",");       // 29
  char* ip4 = strtok(NULL, ",");       // 15

  sprintf(*ip, "%s.%s.%s.%s", ip1, ip2, ip3, ip4);
  
  char* p1 = strtok(NULL, ",");       // 199
  char* p2 = strtok(NULL, ")");       // 78

  *port = atoi(p1)*256 + atoi(p2);
}
  
void saveFile(int socketfd){
  char buf[1024];
  while (read(socketfd, buf, 1024) > 0)
  {
    printf("%s", buf);
  }
  
}
