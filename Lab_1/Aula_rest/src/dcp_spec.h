/** \addtogroup Data_Connection_Protocol_Specification
 *  @{
 */
#ifndef PLD_SPEC_HEADER
#define PLD_SPEC_HEADER

#include "dcp.h"

enum readingType {openR, readR, closeDISC, closeUA}; // Enum of possible modifications to readingCycle()
enum writingType {trans_SET, writeR, trans_DISC_UA}; // Enum of possible modifications to writeCycle()

/**
 * @brief Function to read a byte from fd
 * 
 * @param type variable to distinguish warning messages
 * @param fd file descriptor
 * @param c controll byte, used with type readR
 * @param dataBuf buffer to read data, used with type readR
 * @param retBufferSize variable to store dataBuf size, used with type readR
 * @return int 
 */
int readingCycle(enum readingType type, int fd, unsigned char *c, unsigned char **dataBuf, int *retBufferSize);
/**
 * @brief Function used to write to fd
 * 
 * @param type variable to distinguish warning messages
 * @param fd file descriptor
 * @param buf buffer of content to write
 * @param bufsize lenght of buffer in bytes
 * @return int negative in case of errors, 0 otherwise
 */
int writeCycle(enum writingType type, int fd, unsigned char *buf, int bufsize);

/**
 * @brief Estabelece ligação ao cabo e cria fd
 * 
 * @param fd file descriptor da ligação
 * @param port "/dev/ttySx"
 */
int initConnection(int *fd, char *port);
/**
 * @brief Função handler do sinal de alarme
 * 
 */
void atende();
/**
 * @brief Função que envia Trama SET e recebe trama UA
 * 
 * @param fd identificador da ligação de dados
 * @return int identificador da ligação de dados OU -1 em caso de erro 
 */
int transmitter_SET(int fd);
/**
 * @brief Função que recebe trama SET e envia trama UA
 * 
 * @param fd identificador da ligação de dados
 * @return int identificador da ligação de dados OU -1 em caso de erro 
 */
int receiver_UA(int fd);

/**
 * @brief Stuffs the final buffer with the Information Packet
 * 
 * @param finalBuffer pointer to the Information buffer
 * @param headerBuf pointer to the header buffer [FLAG, A, C, BCC1]
 * @param footerBuf pointer to the footer buffer [BCC2, FLAG]
 * @param footerBufSize size of footer buffer
 * @param dataBuffer pointer to the Data Buffer [D1, D2, ..., Dn]
 * @param dataSize size of Data Buffer
 * @return int size of finalBuffer
 */
int fillFinalBuffer(unsigned char* finalBuffer, unsigned char* headerBuf, unsigned char* footerBuf, int footerBufSize, unsigned char* dataBuffer, int dataSize);

/**
 * @brief Transmitter sequence to Disconnect
 * 
 * @param fd file descriptor ot the connection
 * @return int -1 in case of errors, 0 otherwise
 */
int transmitter_DISC_UA(int fd);

/**
 * @brief Receiver sequence to Disconnect
 * 
 * @param fd file descriptor ot the connection
 * @return int -1 in case of errors, 0 otherwise
 */
int receiver_DISC_UA(int fd);

/**
 * @brief Generates a error on BCC2 based on PROBABILITY_BCC2
 * 
 * @param frame frame to generate error on
 * @param frameSize size of frame 
 */
void generateErrorBCC2(unsigned char *frame, int frameSize);

/**
 * @brief Generates a error on BCC based on PROBABILITY_BCC2
 * 
 * @param checkBuffer Buffer with Address Byte and Control Byte to generate error on
 */
void generateErrorBCC1(unsigned int *checkBuffer);

#endif // PLD_SPEC_HEADER
/** @}*/
