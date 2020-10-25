#ifndef LOGS_HEADER
#define LOGS_HEADER

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>


#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

/**
 * @brief Apresenta uma mensagem a vermelho
 * 
 * @param arr texto a apresentar
 */
void log_error(char *arr);

/**
 * @brief Apresenta uma mensagem a amarelo
 * 
 * @param arr texto a apresentar
 */
void log_caution(char *arr);

/**
 * @brief Apresenta uma mensagem a verde
 * 
 * @param arr texto a apresentar
 */
void log_success(char *arr);

/**
 * @brief Apresenta uma mensagem a branco
 * 
 * @param arr texto a apresentar
 */
void log_message(char *arr);

/**
 * @brief Apresenta uma mensagem na forma "mensagem - numero"
 * 
 * @param arr string de mensagem
 * @param numb numero 
 */
void log_message_number(char *arr, int numb);

/**
 * @brief Apresenta uma mensagem na forma "Content: 0xHH"
 * 
 * @param a byte a apresentar
 */
void log_hexa(unsigned char a);

/**
 * @brief Apresenta a mensagem recebida pelo llread()
 * 
 * @param buf Buffer da mensagem
 * @param size Tamanho da mensagem
 */
void log_received_message(unsigned char *buf, int size);

#endif // LOGS_HEADER