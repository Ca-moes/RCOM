#include "logs.h"

void log_error(char *arr){
  fprintf(stdout, RED "%s\n" RESET, arr);
}

void log_caution(char *arr){
  fprintf(stdout, YELLOW "%s\n" RESET, arr);
}

void log_success(char *arr){
  fprintf(stdout, GREEN "%s\n" RESET, arr);
}

void log_message(char *arr){
  fprintf(stdout, "%s\n" , arr);
}

void log_message_number(char *arr, int numb){
  fprintf(stdout, "%d - %s\n" , numb,  arr);
}

void log_hexa(unsigned char a){
  fprintf(stdout, "Content: %#4.2x\n" , a);
}
