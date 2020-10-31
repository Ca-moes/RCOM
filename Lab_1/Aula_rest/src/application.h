/** \addtogroup Application
 *  @{
 */
#ifndef APP_HEADER
#define APP_HEADER

#include "dcp.h"
#include "logs.h"

/**
 * @brief Struct to save data relative to the Application
 */
struct applicationLayer{
  off_t filesize; // Size of file in bytes
  char filename[255];  // String with file name
  char destinationArg[255];  // String with path to destination message received
  char filenameArg[255]; //String with filename passed by programs arguments
  int type; // TRANSMITTER | RECEIVER
  int gate; // /dev/ttySx | gate is x
};

/**
 * @brief Função para dar parse dos argumentos da linah de comandos
 * 
 * @param argc argc de main
 * @param argv argv de main
 */
void parseArgs(int argc, char** argv);

/**
 * @brief Transmitter part of the application
 * 
 * @param fd file descriptor of connection
 * @return int 0 if all okay, negative otherwise
 */
int transmitterApp(int fd);

/**
 * @brief Parses the info of a Control Package to the Application Layer Struct
 * 
 * @param controlpackage Control Package
 */
void parseFileInfo(unsigned char *controlpackage, int packagesize);

/**
 * @brief Receiver part of the application
 * 
 * @param fd file descriptor of connection
 * @return int 0 if all okay, negative otherwise
 */
int receiverApp(int fd);

/**
 * @brief 
 * 
 * @param argc valor esperado: 3
 * @param argv argv[1] = receiver|transmitter argv[2]={0,1,10,11}
 * @return int 
 */
int main(int argc, char** argv);

#endif // APP_HEADER
/** @}*/
