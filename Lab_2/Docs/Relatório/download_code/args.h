#ifndef ARGS_HEADER
#define ARGS_HEADER

#define h_addr h_addr_list[0]	//The first address in h_addr_list. 

#include <stdio.h>
#include <string.h>
#include <netdb.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/**
 * @brief Struct that keeps the data from the URL argument
 */
typedef struct args
{
  char user[128];       ///< User used for Login
  char password[128];   ///< Password used for Login
  char host[256];       ///< Host name in URL
  char path[240];       ///< Path to the file
  char file_name[128];  ///< Name of the File
  char host_name[128];  ///< Host Name from gethostbyname()
  char ip[128];         ///< IP Adress from gethostbyname()

} args;

/**
 * @brief Parses the URL received via arguments to the struct args
 * 
 * @param url string received through the arguments of download
 * @param args struct args where the information will be saved
 * @return int Returns 0 if there are no errors or a positive number otherwise
 */
int parseArgs(char *url, args * args);

/**
 * @brief Given a host name returns the ip address using gethostbyname()
 * 
 * @param host String with the host's name
 * @param args struct args to put the IP Adress in
 * @return int Returns 0 if there are no errors or 1 otherwise
 */
int getIp(char *host, args * args);

/**
 * @brief Gets the last element of the path string, corresponding to the file name
 * 
 * @param args arguments struct to get path and fill filename
 * @return int Returns 0 if there are no errors or 1 otherwise
 */
int getFileName(args * args);
#endif // ARGS_HEADER
