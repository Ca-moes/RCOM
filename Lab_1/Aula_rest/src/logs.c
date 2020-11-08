/** \addtogroup Logs
 *  @{
 */
#define _POSIX_C_SOURCE 199309L
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

void clearProgressBar() {
    int i;
    for (i = 0; i < NUM_BACKSPACES+1; ++i)
        fprintf(stdout, "\b");
    fflush(stdout);
}

void printProgressBar(int progress, int total) {
    int i, percentage = (int)((((double)progress) / total) * 100);
    int num_separators = (int)((((double)progress) / total) * PROGRESS_BAR_SIZE);;
    fprintf(stdout, "[");
    for (i = 0; i < num_separators; ++i) 
      fprintf(stdout, "%c", SEPARATOR_CHAR);
    for (; i < PROGRESS_BAR_SIZE; ++i) 
      fprintf(stdout, "%c", EMPTY_CHAR);
    
    fprintf(stdout, "]  %2d%% ", percentage);
    fflush(stdout);
}

void startTime(struct timespec *start_time){
  if(clock_gettime(CLOCK_MONOTONIC, start_time) == -1){perror("startTime");}
}

void log_elapsedTime(char * message, struct timespec start_time){
  struct timespec current_time;
  if(clock_gettime(CLOCK_MONOTONIC, &current_time) == -1){perror("elapsedTime");}
  double elapsed = (current_time.tv_sec-start_time.tv_sec)*1000+((current_time.tv_nsec-start_time.tv_nsec)/10e6);
  
  char msg[255];
  sprintf(msg,"%s %f ms\n", message,elapsed);
  write(STDOUT_FILENO, msg, strlen(msg));
}

void log_bitsPerSecond(double nbytes, struct timespec start_time){
  struct timespec current_time;
  if(clock_gettime(CLOCK_MONOTONIC, &current_time) == -1){perror("elapsedTime");}
  double elapsed = (current_time.tv_sec-start_time.tv_sec)*1000+((current_time.tv_nsec-start_time.tv_nsec)/10e6);
  
  double bits = (nbytes * 8) / (elapsed/1000);

  char msg[255];
  sprintf(msg,"%s %f \n", "[R] Bits per second: ", bits);
  write(STDOUT_FILENO, msg, strlen(msg));
}
/** @}*/
