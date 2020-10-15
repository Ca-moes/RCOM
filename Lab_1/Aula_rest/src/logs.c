#include "logs.h"

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
