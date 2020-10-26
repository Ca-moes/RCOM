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

/**
 * @brief Struct to hold information about the lin layer
 */
struct linkLayer {
  char port[20]; /* Device /dev/ttySx, x = 0, 1*/
  int baudRate; /*Transmission Speed*/
  unsigned char sequenceNumber;   /*Frame Sequence Number: 0x00, 0x01*/
  unsigned int timeout; /*Alarm Timeout: x s*/
  unsigned int numTransmissions; /*Number of tries in case of failure*/
  unsigned int status; /*TRANSMITTER | RECEIVER*/
  unsigned char frame[MAX_SIZE+6]; /*Frame*/
};
enum readingType {openR, readR, closeDISC, closeUA}; // Enum of possible modifications to readingCycle()
enum writingType {trans_SET, writeR, trans_DISC_UA}; // Enum of possible modifications to writeCycle()

struct linkLayer linkLayer;  // Data relative to the link layer
struct termios oldtio; // used in llclose to reset termios
volatile int failed; // used in the alarm handler

/**
 * @brief Function to read a byte from fd
 * 
 * @param type variable to distinguish warning messages
 * @param fd file descriptor
 * @param c controll byte, used with type readR
 * @param dataBuf buffer to read data, used with type readR
 * @param retBufferSize variable to store dataBuf size, used with type readR
 * @return int 
 */
int readingCycle(enum readingType type, int fd, unsigned char *c, unsigned char **dataBuf, int *retBufferSize);
/**
 * @brief Function used to write to fd
 * 
 * @param type variable to distinguish warning messages
 * @param fd file descriptor
 * @param buf buffer of content to write
 * @param bufsize lenght of buffer in bytes
 * @return int negative in case of errors, 0 otherwise
 */
int writeCycle(enum writingType type, int fd, unsigned char *buf, int bufsize);
/**
 * @brief Estabelece ligação ao cabo e cria fd
 * 
 * @param fd file descriptor da ligação
 * @param port "/dev/ttySx"
 */
int initConnection(int *fd, char *port);
/**
 * @brief Função handler do sinal de alarme
 * 
 */
void atende();
/**
 * @brief Função que envia Trama SET e recebe trama UA
 * 
 * @param fd identificador da ligação de dados
 * @return int identificador da ligação de dados OU -1 em caso de erro 
 */
int transmitter_SET(int fd);
/**
 * @brief Função que recebe trama SET e envia trama UA
 * 
 * @param fd identificador da ligação de dados
 * @return int identificador da ligação de dados OU -1 em caso de erro 
 */
int receiver_UA(int fd);
/**
 * @brief Abre uma ligação de dados com a porta série
 * 
 * @param porta número da porta x em "/dev/ttySx"
 * @param type TRANSMITTER|RECEIVER
 * @return int identificador da ligação de dados OU -1 em caso de erro
 */
int llopen(int porta, int type);


/**
 * @brief Stuffs the final buffer with the Information Packet
 * 
 * @param finalBuffer pointer to the Information buffer
 * @param headerBuf pointer to the header buffer [FLAG, A, C, BCC1]
 * @param footerBuf pointer to the footer buffer [BCC2, FLAG]
 * @param dataBuffer pointer to the Data Buffer [D1, D2, ..., Dn]
 * @param dataSize dize of Data Buffer
 * @return int size of finalBuffer
 */
int fillFinalBuffer(unsigned char* finalBuffer, unsigned char* headerBuf, unsigned char* footerBuf, unsigned char* dataBuffer, int dataSize);
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