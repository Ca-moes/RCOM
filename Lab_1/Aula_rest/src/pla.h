#ifndef PLA_HEADER
#define PLA_HEADER


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

#include "macros.h"
#include "logs.h"

#define BAUDRATE B38400

// valores de type usados em application.c
#define TRANSMITTER 1
#define RECEIVER 0

#define FALSE 0
#define TRUE 1

/**
 * @brief Estabelece ligação ao cabo e cria fd
 * 
 * @param fd file descriptor da ligação
 * @param port "/dev/ttySx"
 */
void initConnection(int *fd, char *port);
/**
 * @brief Função handler do sinal de alarme
 * 
 */
void atende();
/**
 * @brief Função para processar o estado da leitura das tramas SET-UA
 * 
 * @param state estado atual da máquina de estados SET-UA
 * @param checkBuffer Buffer que guarda os valores dos bytes A e C
 * @param byte Byte a ser processado pela máquina de estados
 * @param type TRANSMITTER|RECEIVER - Tipo da máquina de estados 
 */
void stateMachine_SET_UA(enum stateMachine *state, unsigned char *checkBuffer, char byte, int type);
/**
 * @brief Função que envia Trama SET e recebe trama UA
 * 
 * @param fd identificador da ligação de dados
 */
void transmitter_SET(int fd);
/**
 * @brief Função que recebe trama SET e envia trama UA
 * 
 * @param fd identificador da ligação de dados
 */
void receiver_UA(int fd);

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
int llread(int fd, char *buffer);
/**
 * @brief 
 * 
 * @param fd identificador da ligação de dados
 * @return int valor positivo em caso de sucesso ou -1 em caso de erro
 */
int llclose(int fd);


#endif // PLA_HEADER