#include "download.h"

int main(int argc, char** argv){ 

  args arguments;

  if (parseArgs(argv[1], &arguments) != 0){
    printf("usage: %s ftp://[<user>:<password>@]<host>/<url-path>\n",argv[0]); 
		return -1;
  }

  printf("\nhost: %s\npath: %s\nuser: %s\npassword: %s\nfile name: %s\nhost name: %s\nip address: %s\n", 
  arguments.host, arguments.path, arguments.user, arguments.password, arguments.file_name, arguments.host_name, arguments.ip);
  
  struct	sockaddr_in server_addr;
	int	sockfd;

  /*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(arguments.ip);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(23);		/*server TCP port must be network byte ordered */
    
	/*open an TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    perror("socket()");
    return 1;
  }

  printf("sockfd1: %d\n", sockfd);

	/*connect to the server*/
  if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    perror("connect()");
		return 1;
	}

  printf("sockfd2: %d\n", sockfd);



  return 0;
}