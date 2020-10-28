#ifndef PLA_HEADER
#define PLA_HEADER
#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "macros.h"
#include "logs.h"
#include "statemachine.h"
#include "dcp_spec.h"

/**
 * @brief Struct to hold information about the Link Layer
 */
struct linkLayer {
  char port[20]; /* Device /dev/ttySx, x = 0, 1*/
  int baudRate; /*Transmission Speed*/
  unsigned char sequenceNumber;   /*Frame Sequence Number: 0x00, 0x01*/
  unsigned int timeout; /*Alarm Timeout: x s*/
  unsigned int numTransmissions; /*Number of tries in case of failure*/
  unsigned char frame[MAX_SIZE_AFT_STUFF]; /*Frame with double the size of MAX_SIZE to accomodate for byte_stuffing increase*/
  unsigned int status; /*TRANSMITTER | RECEIVER*/
};


struct linkLayer linkLayer;  // Data relative to the link layer
struct termios oldtio; // used in llclose to reset termios
volatile int failed; // used in the alarm handler


/**
 * @brief Abre uma ligação de dados com a porta série
 * 
 * @param porta número da porta x em "/dev/ttySx"
 * @param type TRANSMITTER|RECEIVER
 * @return int identificador da ligação de dados OU -1 em caso de erro
 */
int llopen(int porta, int type);

/**
 * @brief 
 * 
 * @param fd identificador da ligação de dados
 * @param buffer array de caracteres a transmitir
 * @param lenght comprimento do array de caracteres
 * @return int numero de caracteres escritos ou -1 em caso de erro
 */
int llwrite(int fd, char *buffer, int lenght);

/**
 * @brief 
 * 
 * @param fd identificador da ligação de dados
 * @param buffer array de caracteres recebidos
 * @return int comprimento do array (número de caracteres lidos) ou -1 em caso de erro
 */
int llread(int fd, unsigned char *buffer);

/**
 * @brief 
 * 
 * @param fd identificador da ligação de dados
 * @return int valor positivo em caso de sucesso ou -1 em caso de erro
 */
int llclose(int fd);

#endif // PLA_HEADER
