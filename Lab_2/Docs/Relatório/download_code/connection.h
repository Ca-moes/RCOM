#ifndef CONNECT_HEADER
#define CONNECT_HEADER

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "args.h"
FILE * socketFile;

/**
 * @brief Opens a socket and connects to the server
 * 
 * @param ip Server IP Adress
 * @param port Server Port
 * @param socketfd File Descriptor returned by socket()
 * @return int Returns 0 if there are no errors or a positive number otherwise
 */
int init(char *ip, int port, int *socketfd);

/**
 * @brief Sends a message to the server via socket
 * 
 * @param socketfd File Descriptor of the socket
 * @param command String with the command to send
 * @return int Returns 0 if there are no errors or a positive number otherwise
 */
int sendCommand(int socketfd, char * command);

/**
 * @brief Prints to the standart output the reply received after sending a command to empty the reply buffer
 * 
 * @return int Returns 0
 */
int readResponse();

/**
 * @brief Reads the reply to the "pasv" command ans parses the IP Adress and Port Values
 * 
 * @param ip IP Adress
 * @param port Server Port
 * @return int Returns 0
 */
int readResponsePassive(char (*ip)[], int *port);

/**
 * @brief While reading the reply from the served, dumps the information received into a file
 * 
 * @param filename Name of the File to be created
 * @param socketfd Socket File Descriptor from where to read the content
 * @return int Returns 0 if there are no errors or a positive number otherwise
 */
int saveFile(char* filename, int socketfd);
#endif // CONNECT_HEADER
