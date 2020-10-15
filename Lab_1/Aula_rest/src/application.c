#include "pla.h"

// Aplicação receberá como argumento em main se é Receiver ou Sender
// com isto abrirá llopen() de forma diferente 
int main(int argc, char** argv) {
  int fd;

  printf("Started App\n");
  fd = llopen(10, RECEIVER);
  printf("Open Successful. Closing..\n");
  llclose(fd);
}