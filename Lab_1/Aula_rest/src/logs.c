#include "logs.h"

void log_error(char *arr){
  char msg[255];
  sprintf(msg, RED "%s\n" RESET, arr);
  write(STDOUT_FILENO, msg, strlen(msg));
}

void log_caution(char *arr){
  char msg[255];
  sprintf(msg, YELLOW "%s\n" RESET, arr);
  write(STDOUT_FILENO, msg, strlen(msg));
}

void log_success(char *arr){
  char msg[255];
  sprintf(msg, GREEN "%s\n" RESET, arr);
  write(STDOUT_FILENO, msg, strlen(msg));
}

void log_message(char *arr){
  char msg[255];
  sprintf(msg, "%s\n" , arr);
  write(STDOUT_FILENO, msg, strlen(msg));
}

void log_message_number(char *arr, int numb){
  char msg[255];
  sprintf(msg, "%d - %s\n" , numb,  arr);
  write(STDOUT_FILENO, msg, strlen(msg));
}

void log_hexa(unsigned char a){
  char msg[255];
  sprintf(msg, "Content: %#4.2x\n" , a);
  write(STDOUT_FILENO, msg, strlen(msg));
}

/* To test if cheanged;
  log_message_number("oi oi teste", 11);
  log_error("error message");
  log_caution("caution message");
  log_success("sucess message");
  log_message("mensagem");
  log_hexa(0x7E); */