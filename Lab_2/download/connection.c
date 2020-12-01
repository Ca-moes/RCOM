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

  printf("socketfd: %d\n", *socketfd);
  return 0;
}