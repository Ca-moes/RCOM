#ifndef PLA_HEADER
#define PLA_HEADER
#define _GNU_SOURCE
/** \addtogroup Data_Connection_Protocol
 *  @{
 */
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
#include <errno.h>

#include "macros.h"
#include "logs.h"
#include "statemachine.h"
#include "dcp_spec.h"

/**
 * @brief Struct to hold information about the Link Layer
 */
struct linkLayer {
  char port[20]; /* Device /dev/ttySx, x = 0, 1*/
  int baudRate; /*Transmission Speed*/
  unsigned char sequenceNumber;   /*Frame Sequence Number: 0x00, 0x01*/
  unsigned int timeout; /*Alarm Timeout: x s*/
  unsigned int numTransmissions; /*Number of tries in case of failure*/
  unsigned char frame[MAX_SIZE_AFT_STUFF]; /*Frame with double the size of MAX_SIZE to accomodate for byte_stuffing increase*/
  unsigned int status; /*TRANSMITTER | RECEIVER*/
};


struct linkLayer linkLayer;  ///< Data relative to the link layer
struct termios oldtio; ///< used in llclose to reset termios
volatile int failed; ///< used in the alarm handler


/**
 * @brief Opens a data connection with the serial port
 *  
 * @param porta number of the port x in  "/dev/ttySx"
 * @param type TRANSMITTER|RECEIVER
 * @return int idata connection identifier or -1 in case of error
 */
int llopen(int porta, int type);

/**
 * @brief Writes the content of buffer to fd
 * 
 * @param fd data connection identifier
 * @param buffer character array of data to send
 * @param lenght array's size
 * @return int amount of characters read or -1 in case of error
 */
int llwrite(int fd, char *buffer, int lenght);

/**
 * @brief Reads from fd to buffer 
 * 
 * @param fd data connection identifier
 * @param buffer character array of received data
 * @return int array's size or -1 in case of error 
 */
int llread(int fd, unsigned char *buffer);

/**
 * @brief Closes the connection to the serial port
 * 
 * @param fd data connection identifier
 * @return int positive value in case of sucess, -1 in case of error
 */
int llclose(int fd);

#endif // PLA_HEADER
/** @}*/
