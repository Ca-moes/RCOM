/** \addtogroup Logs
 *  @{
 */
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
 * @brief Outputs a message in red
 * 
 * @param arr message String
 */
void log_error(char *arr);

/**
 * @brief Outputs a message in yellow
 * 
 * @param arr message String
 */
void log_caution(char *arr);

/**
 * @brief Outputs a message in green
 * 
 * @param arr message String
 */
void log_success(char *arr);

/**
 * @brief Outputs a message in white
 * 
 * @param arr message String
 */
void log_message(char *arr);

/**
 * @brief Outputs a message in the form "number - message"
 * 
 * @param arr message String
 * @param numb number
 */
void log_message_number(char *arr, int numb);

/**
 * @brief Outputs a message in the form "Content: 0xHH"
 * 
 * @param a Byte
 */
void log_hexa(unsigned char a);

/**
 * @brief Outputs a message (String) received by llread
 * 
 * @param buf message Buffer
 * @param size Size of the message
 */
void log_received_message(unsigned char *buf, int size);

#endif // LOGS_HEADER
/** @}*/
