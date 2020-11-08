/** \addtogroup Logs
 *  @{
 */
#ifndef LOGS_HEADER
#define LOGS_HEADER

#include <time.h>
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


#define PROGRESS_BAR_SIZE   30
#define SEPARATOR_CHAR      '#'
#define EMPTY_CHAR          '.'
#define NUM_BACKSPACES      PROGRESS_BAR_SIZE + 9

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
 * @brief Clears progress bar from terminal
 * 
 */
void clearProgressBar();

/**
 * @brief Prints progress bar on terminal
 * 
 * @param progress nbytes already sent
 * @param total number of bytes to send
 */
void printProgressBar(int progress, int total);

/**
 * @brief starts counting time
 * 
 */
void startTime(struct timespec *start_time);

/**
 * @brief starts counting time
 * 
 */
void log_elapsedTime(char * message, struct timespec start_time);

void log_bitsPerSecond(double nbytes, struct timespec start_time);

#endif // LOGS_HEADER
/** @}*/
