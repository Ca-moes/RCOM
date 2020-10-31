/** \addtogroup Application
 *  @{
 */
#include "application.h"

static struct applicationLayer applayer;  // Struct to save data about the Application

void clearProgressBar() {
    int i;
    for (i = 0; i < NUM_BACKSPACES; ++i) {
        fprintf(stdout, "\b");
    }
    fflush(stdout);
}

void printProgressBar(int progress, int total) {
    int i, percentage = (int)((((double)progress) / total) * 100);
    int num_separators = (int)((((double)progress) / total) * PROGRESS_BAR_SIZE);;
    fprintf(stdout, "[");
    for (i = 0; i < num_separators; ++i) {
        fprintf(stdout, "%c", SEPARATOR_CHAR);
    }
    for (; i < PROGRESS_BAR_SIZE; ++i) {
        fprintf(stdout, "%c", EMPTY_CHAR);
    }
    fprintf(stdout, "]  %2d%%  ", percentage);
    fflush(stdout);
}

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

int transmitterApp(int fd){
  char controlPackage[255];
  char dataPackage[MAX_SIZE];
  //char dataPackageTest[MAX_SIZE];
  //int counterTest = 0;

  int sequenceNumber = 0;
  int nbytes;
  int file_fd;
  char * file_data[MAX_SIZE];

  struct stat fileInfo;

  if (stat(applayer.filenameArg, &fileInfo)<0){
    log_error("transmitterApp() - Error obtaining file information.\n");
    return -1;
  }

  file_fd = open(applayer.filenameArg,O_RDONLY);
  if (file_fd <0){
    log_error("transmitterApp() - Error opening file.\n");
    return -1;
  }

  /*building control package*/
  controlPackage[0] = START;
  controlPackage[1] = T_SIZE;
  controlPackage[2] = sizeof(fileInfo.st_size); /* sending size of off_t */
  memcpy(&controlPackage[3],&fileInfo.st_size,sizeof(fileInfo.st_size));

  controlPackage[sizeof(fileInfo.st_size)+3] = T_NAME;
  controlPackage[sizeof(fileInfo.st_size)+4] = strlen(applayer.filenameArg);
  memcpy(&controlPackage[sizeof(fileInfo.st_size)+5],applayer.filenameArg,strlen(applayer.filenameArg));

  if(llwrite(fd,controlPackage,sizeof(fileInfo.st_size) + 5 + strlen(applayer.filenameArg))<0){
    log_error("transmitterApp() - llwrite() failed writing Start Control Packet");
    return -1;
  }
  int progress = 0;

  while( (nbytes = read(file_fd,file_data,MAX_SIZE-4)) != 0){
    /*building data package*/
    dataPackage[0] = DATA;
    dataPackage[1] = sequenceNumber % 255;
    dataPackage[2] = nbytes / 256;
    dataPackage[3] = nbytes % 256;
    memcpy(&dataPackage[4],file_data,nbytes);

    /*if (sequenceNumber == 2){
      memcpy(dataPackageTest, dataPackage, MAX_SIZE);
    }*/

    progress+=nbytes;
    printProgressBar(progress,fileInfo.st_size);

    if (llwrite(fd,dataPackage,nbytes+4) < 0){
      log_error("transmitterApp() - llwrite() failed writing Data Packet");
      return -1;
    }
    sequenceNumber++;
    //counterTest++;

    /*if (counterTest == 5){
      llwrite(fd,dataPackageTest,nbytes+4);
    }*/

    clearProgressBar();

  }

  printProgressBar(1,1);

  controlPackage[0] = END;

  if(llwrite(fd,controlPackage,sizeof(fileInfo.st_size) + 5 + strlen(applayer.filenameArg))<0){
    log_error("transmitterApp() - llwrite() failed writing End Control Packet");
    return -1;
  }
  return 0;
}

void parseFileInfo(unsigned char *controlpackage, int packagesize){
  off_t size = 0;
  int informationsize;
  int next_tlv = 1;

  while (next_tlv != packagesize)
  {
    if (controlpackage[next_tlv]==T_SIZE){
      informationsize = controlpackage[next_tlv+1];
      for (int i = next_tlv+2, k=0; i < informationsize+next_tlv+2; i++, k++){
        size += controlpackage[i] << 8*k;
      }
      applayer.filesize = size;
      next_tlv=informationsize+3;
    }

    if (controlpackage[next_tlv]==T_NAME){
      informationsize = controlpackage[next_tlv+1];
      for (int i =next_tlv+2, k=0; i < informationsize+next_tlv+2; i++, k++){
        applayer.filename[k] = controlpackage[i];
      }
      applayer.filename[informationsize+next_tlv+2] = '\0';
      next_tlv = next_tlv+2 + informationsize;
    }
  }
}

int receiverApp(int fd){
  unsigned char package[MAX_SIZE];
  int dataPackageSize;
  int nsequence_expected=0, nsequence_expected_aux;
  int offset_need = 0;

  int file_fd, sizeread;

  while(1){
    sizeread = llread(fd,package);
    
    if(sizeread<0)
      log_caution("receiverApp() - llread() error");

    if (package[0] == START){
      parseFileInfo(package, sizeread);
      strcat(applayer.destinationArg,applayer.filename);
      file_fd = open(applayer.destinationArg,O_RDWR | O_CREAT, 0777);
      continue;
    }
    else if (package[0] == END){
      break;
    }
    else if (package[0] == DATA && sizeread>0) {
      dataPackageSize = package[3] + 256* package[2];

      if (package[1] != nsequence_expected){
        off_t offset = (package[1] + offset_need) * (MAX_SIZE-4) ;
        lseek(file_fd, offset, SEEK_SET);
      }
      
      write(file_fd,&package[4], dataPackageSize);

      if (package[1] != nsequence_expected){
        lseek(file_fd, 0 , SEEK_HOLE);
      }

      if (package[1]== nsequence_expected){
        nsequence_expected++;
      }
      
      nsequence_expected_aux = nsequence_expected;
      nsequence_expected %= 255;

      if (nsequence_expected % 255 == 0 && nsequence_expected_aux!= 0){
        offset_need+=255;
      }
    }
  }

  if (close(file_fd)<0){
    log_error("Error closing file\n");
  }
  
  return fd;
}

int main(int argc, char** argv) {
  printf("Started App\n");

  int fd;
  parseArgs(argc, argv);

  fd = llopen(applayer.gate, applayer.type);

  if (fd < 0) {
    log_error("main() - Unable to establish connection. Exiting..  ");
    return -1;
  } else log_success("Connection established.");

  
  if (applayer.type == TRANSMITTER) 
  {
    if( transmitterApp(fd) < 0)
      log_error("main() - transmitterApp error");
  }
  else if (applayer.type == RECEIVER) {
    if (receiverApp(fd) <0)
      log_error("main() - receiverApp error");
  }
  
  printf("Closing Connection..\n");
  if (llclose(fd) < 0){
    log_error("main() - Error on llclose()");
    return -1;
  }
  return 0;
}
/** @}*/
