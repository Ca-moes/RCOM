#ifndef LOGS_HEADER
#define LOGS_HEADER

#include <stdio.h>

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

// prints "message" in red!
void log_error(char *arr){
  fprintf(stdout, RED "%s\n" RESET, arr);
}

// prints "message" in yellow!
void log_caution(char *arr){
  fprintf(stdout, YELLOW "%s\n" RESET, arr);
}

// prints "message" in green!
void log_success(char *arr){
  fprintf(stdout, GREEN "%s\n" RESET, arr);
}

// prints "message" in normal white
void log_message(char *arr){
  fprintf(stdout, "%s\n" , arr);
}

// prints "message - numb"
void log_message_number(char *arr, int numb){
  fprintf(stdout, "%d - %s\n" , numb,  arr);
}

// prints "Content: 0xHH"
void log_hexa(unsigned char a){
  fprintf(stdout, "Content: %#4.2x\n" , a);
}

#endif // LOGS_HEADER