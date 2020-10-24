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


struct linkLayer {
  char port[20]; /*Dispositivo /dev/ttySx, x = 0, 1*/
  int baudRate; /*Velocidade de transmissão*/
  unsigned char sequenceNumber;   /*Número de sequência da trama: 0x00, 0x01*/
  unsigned int timeout; /*Valor do temporizador: 1 s*/
  unsigned int numTransmissions; /*Número de tentativas em caso defalha*/
  unsigned char frame[MAX_SIZE]; /*Trama*/
  unsigned int status; /*TRANSMITTER | RECEIVER*/
};

struct linkLayer linkLayer;

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
 * @brief Função para processar o estado da leitura das tramas SET-UA
 * 
 * @param state estado atual da máquina de estados SET-UA
 * @param checkBuffer Buffer que guarda os valores dos bytes A e C
 * @param byte Byte a ser processado pela máquina de estados
 * @return -1 if BCC's don't match, -2 if SequenceNumber is wrong. 0 otherwise
 */
int stateMachine_Read(unsigned char byte, unsigned char **buffer, int* buffersize);

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