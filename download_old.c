#include "download.h"

int connect_ftp(char * url,int port)
{
	int	socketfd;
	struct sockaddr_in server_addr;
	
	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(getHostIp(url)); /* getHostIp faz ns lookup*/	
	server_addr.sin_port = htons(port);	
    
	/*open an TCP socket*/
	if ((socketfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("socket()");
		exit(0);
    }

	/*connect to the server*/
    if(connect(socketfd, (struct sockaddr *)&server_addr,  sizeof(server_addr)) < 0){
        perror("connect()");
		exit(0);
	}
	return socketfd;
}

char* getHostIp(char * url)
{
	struct hostent *h;
    if ((h=gethostbyname(url)) == NULL) {  
        herror("gethostbyname");
        exit(1);
    }
    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n",inet_ntoa(*((struct in_addr *)h->h_addr)));
    return inet_ntoa(*((struct in_addr *)h->h_addr));
}

int main(int argc, char** argv){
	char *user, *password, *host, *path;
	if(argc!=2){
		printf("usage: %s ftp://[<user>:<password>@]<host>/<url-path>\n",argv[0]); 
		return -1;
	}

	char *barra = strchr(argv[1] + 6,'/');
	if (barra == NULL){
		printf("usage: %s ftp://[<user>:<password>@]<host>/<url-path>\n",argv[0]); 
		return -1;
	}

  puts(argv[1]);

	*barra = '\0';
	path = barra + 1;

	char *doispontos = strchr(argv[1] + 6, ':');
  puts(doispontos);
	char *arroba = strchr(argv[1] + 6, '@');

  puts(argv[1]);


	if (doispontos == NULL || arroba == NULL){
		user = "user";
		password = "1234";
		host = argv[1] + 6;
	}
	else{
		*doispontos = '\0';
		*arroba = '\0';

    puts(argv[1]);
		user = argv[1] + 6;
		password = doispontos + 1;
		host = arroba + 1;
	}
	
	puts(user);
	puts(password);
	puts(host);
	puts(path);

	/*socketfd = connect_ftp(host,SERVER_PORT);*/

	
	return 1;
}


