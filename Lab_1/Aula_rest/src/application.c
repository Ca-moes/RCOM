#include "pla.h"
#include "logs.h"

/**
 * @brief Função para dar parse dos argumentos da linah de comandos
 * 
 * @param argc argc de main
 * @param argv argv de main
 * @param x valor de x em "/dev/ttySx"
 * @param type RECEIVER|TRANSMITTER
 */
void parseArgs(int argc, char** argv, int* x, int* type){
  if (argc != 3){
    log_error("Usage: ./application [receiver/transmitter] [nPorta = {0,1,10,11}]\n");
    exit(-1);
  }

  *x = atoi(argv[2]);
  if (*x != 0 && *x != 1 && *x != 10 && *x != 11) {
    log_caution("nPorta = {0,1,10,11}\n");
    exit(-1);
  }

  if (strcmp("receiver",argv[1])==0){
    *type = RECEIVER;
  } else if (strcmp("transmitter",argv[1])==0){
    *type = TRANSMITTER;
  } else {
    log_caution("Usage: ./application [receiver/transmitter] [nPorta = {0,1,10,11}]\n");
    exit(-1);
  }
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

  int fd, x, type;
  parseArgs(argc, argv, &x, &type);
  
  fd = llopen(x, type);
  
  if (fd < 0) log_error("Unable to establish connection.");
  else log_success("Connection established.");

  printf("Closing..\n");
  llclose(fd);
  return 0;
}