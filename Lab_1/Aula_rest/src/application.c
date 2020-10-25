#include "pla.h"
#include "logs.h"

struct applicationLayer{
  off_t filesize;
  char *filename;
  char destinationArg[255];
  char filenameArg[255];
  int type;
  int gate;
};

struct applicationLayer applayer;

/**
 * @brief Função para dar parse dos argumentos da linah de comandos
 * 
 * @param argc argc de main
 * @param argv argv de main
 * @param x valor de x em "/dev/ttySx"
 * @param type RECEIVER|TRANSMITTER
 */
void parseArgs(int argc, char** argv){
  if (argc != 4){
    log_error("Usage: ./application (receiver <destination> | transmitter <filename>) [gate = {0,1,10,11}]\n");
    exit(-1);
  }

  applayer.gate = atoi(argv[3]);
  if (applayer.gate != 0 && applayer.gate != 1 && applayer.gate != 10 && applayer.gate != 11) {
      log_caution("nPorta = {0,1,10,11}\n");
      exit(-1);
  }

  if (strcmp("receiver",argv[1])==0){
      applayer.type = RECEIVER;
      strcpy(applayer.destinationArg,argv[2]);
  } 
  else if (strcmp("transmitter",argv[1])==0){
      applayer.type = TRANSMITTER;
      strcpy(applayer.filenameArg,argv[2]);
  } 
  else {
      log_caution("Usage: ./application (receiver <destination> | transmitter <filename>) [gate = {0,1,10,11}]\n");
      exit(-1);
  }
}

int transmitterApp(int fd, char *file){
  char controlPackage[255];
  char dataPackage[MAX_SIZE];
  int sequenceNumber = 0;
  int nbytes;
  int file_fd;
  char * file_data[MAX_SIZE];

  struct stat fileInfo;

  if (stat(file, &fileInfo)<0){
    log_error("Error obtaining file information.\n");
    return -1;
  }

  file_fd = open(file,O_RDONLY);
  if (file_fd <0){
    log_error("Error opening file.\n");
    return -1;
  }

  /*building control package*/
  controlPackage[0] = START;
  controlPackage[1] = T_SIZE;
  controlPackage[2] = sizeof(fileInfo.st_size); /* sending size of off_t */
  memcpy(&controlPackage[3],&fileInfo.st_size,sizeof(fileInfo.st_size));

  controlPackage[sizeof(fileInfo.st_size)+3] = T_NAME;
  controlPackage[sizeof(fileInfo.st_size)+4] = strlen(file);
  memcpy(&controlPackage[sizeof(fileInfo.st_size)+5],file,strlen(file));

  llwrite(fd,controlPackage,sizeof(fileInfo.st_size) + 5 + strlen(file));

  for (int i=0; i< sizeof(fileInfo.st_size)+5 + strlen(file); i++){
    log_hexa(controlPackage[i]);
  }


  while( (nbytes = read(file_fd,file_data,MAX_SIZE-4)) != 0){
    /*building data package*/
    dataPackage[0] = DATA;
    dataPackage[1] = sequenceNumber % 255;
    dataPackage[2] = nbytes / 256;
    dataPackage[3] = nbytes % 256;
    memcpy(&dataPackage[4],file_data,nbytes);

    llwrite(fd,dataPackage,nbytes+4);

    sequenceNumber++;
  }

  controlPackage[0] = END;

  llwrite(fd,controlPackage,sizeof(fileInfo.st_size) + 5 + strlen(file));
  return 0;
}

void parseFileInfo(unsigned char *controlpackage){
  off_t size = 0;
  int informationsize;
  int next_tlv = 0;

  if (controlpackage[1]==T_SIZE){
    informationsize = controlpackage[2];
    for (int i =3, k=0; i < informationsize+3; i++, k++){
      size += controlpackage[i] << 8*k;
    }
    next_tlv=informationsize+3;
  }
  
  if (controlpackage[next_tlv]==T_NAME){
    informationsize = controlpackage[next_tlv+1];
    applayer.filename = malloc(sizeof(char) * (informationsize+1));
    
    for (int i =next_tlv+2, k=0; i < informationsize+next_tlv+2; i++, k++){
        applayer.filename[k] = controlpackage[i];
        log_message(applayer.filename);
    }
    applayer.filename[sizeof(applayer.filename)] = '\0';
  }
  applayer.filesize = size;
}

int receiverApp(int fd){
  unsigned char package[MAX_SIZE];
  int dataPackageSize;

  int file_fd;

  while(1){
    llread(fd,package);
    
    if (package[0] == START){
      parseFileInfo(package);
      strcat(applayer.destinationArg,applayer.filename);
      file_fd = open(applayer.destinationArg,O_RDWR | O_CREAT, 0777);
      continue;
    }
    else if (package[0] == END){
      break;
    }
    else if (package[0] == DATA) {
      dataPackageSize = package[3] + 256* package[2];

      write(file_fd,&package[4], dataPackageSize);
    }
  }
  
  if (close(file_fd)<0){
    log_error("Error closing file\n");
  }
  
  return fd;
}

/**
 * @brief 
 * 
 * @param argc valor esperado: 3
 * @param argv argv[1] = receiver|transmitter argv[2]={0,1,10,11}
 * @return int 
 */
int main(int argc, char** argv) {
  printf("Started App\n");

  int fd;
  parseArgs(argc, argv);

  fd = llopen(applayer.gate, applayer.type);

  if (fd < 0) {
    log_error("Unable to establish connection. Exiting..  ");
    return -1;
  } else log_success("Connection established.");

  
  if (applayer.type == TRANSMITTER) 
    transmitterApp(fd, applayer.filenameArg);
  else if (applayer.type == RECEIVER) {
    receiverApp(fd);
  }
  
  printf("Closing Connection..\n");
  if (llclose(fd) < 0){
    log_error("Error on llclose()");
    return -1;
  }
  return 0;
}