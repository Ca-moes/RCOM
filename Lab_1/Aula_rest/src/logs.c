/** \addtogroup Logs
 *  @{
 */
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
  sprintf(msg, "%d - %s" , numb,  arr);
  write(STDOUT_FILENO, msg, strlen(msg));
}

void log_hexa(unsigned char a){
  char msg[255];
  sprintf(msg, "Content: %#4.2x\n" , a);
  write(STDOUT_FILENO, msg, strlen(msg));
}

void log_received_message(unsigned char *buf, int size){
  printf("Message received from llread() - Size %d:\n", size);
  for (int i = 0; i < size; i++)
    printf("%#4.2x ", buf[i]);
  //buf[size]='\0'; 
  //printf("\nMensagem: %s\n", buf);
}

void clearProgressBar() {
    int i;
    
    for (i = 0; i < NUM_BACKSPACES; ++i) {
        fprintf(stdout, "\b");
    }
    fflush(stdout);
}

void printProgressBar(int progress, int total) {

    if (progress == 1)
      fprintf(stdout, "\b");

    int i, percentage = (int)((((double)progress) / total) * 100);
    int num_separators = (int)((((double)progress) / total) * PROGRESS_BAR_SIZE);;
    fprintf(stdout, "[");
    for (i = 0; i < num_separators; ++i) {
        fprintf(stdout, "%c", SEPARATOR_CHAR);
    }
    for (; i < PROGRESS_BAR_SIZE; ++i) {
        fprintf(stdout, "%c", EMPTY_CHAR);
    }
    fprintf(stdout, "]  %2d%%  ", percentage);
    fflush(stdout);
}

/* To test if cheanged;
  log_message_number("oi oi teste", 11);
  log_error("error message");
  log_caution("caution message");
  log_success("sucess message");
  log_message("mensagem");
  log_hexa(0x7E); */

  /** @}*/
