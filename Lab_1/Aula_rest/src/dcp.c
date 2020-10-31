/** \addtogroup Data_Connection_Protocol
 *  @{
 */
#include "dcp.h"

static struct sigaction old_action; ///< sigaction to restore SIGALRN handler

int llopen(int porta, int type){
  int fd;
  char port[12]; // "/dev/ttyS11\0" <- 12 chars
  snprintf(port, 12, "/dev/ttyS%d", porta);
  
  if (initConnection(&fd, port) < 0){
    log_error("llopen() - Error on initConnection");
    return -1;
  }
  
  // Set Handler for Alarm
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = atende;
  sa.sa_flags = 0;
  if (sigaction(SIGALRM, &sa, &old_action)<0){
    log_error("llopen() - error on sigaction");
    return -1;
  }

  switch (type)
  {
  case TRANSMITTER:
    linkLayer.status = TRANSMITTER;
    return transmitter_SET(fd);
    break;
  case RECEIVER:
    linkLayer.status = RECEIVER; 
    return receiver_UA(fd);
    break;
  default:
    log_error("llopen() - Wrong value for variable type");
    return -1;
    break;
  }

}

int llwrite(int fd, char *buffer, int lenght){
  int currentLenght = lenght;
  unsigned char buf1[4] = {FLAG, A_ER, C_I(linkLayer.sequenceNumber), BCC(A_ER, C_I(linkLayer.sequenceNumber))};  
  unsigned char *dataBuffer = (unsigned char *)malloc(lenght);

  if (lenght > MAX_SIZE){
    log_error("llwrite() - Message size greater than MAX_SIZE");
    return -1;
  }

  /*building trama I*/
  unsigned char BCC2 = buffer[0];
  for (int i = 1; i<lenght; i++){
    BCC2 = BCC2 ^ buffer[i];
  }

  unsigned char *buf2 = (unsigned char *)malloc(2);
  int buf2Size = 2;
  // Byte Stuffing (BCC2 on buf2)
  if (BCC2 == 0x7E || BCC2 == 0x7D){
    buf2 = (unsigned char *) realloc(buf2, 3);
    buf2[0] = 0x7D;
    buf2[1] = BCC2 ^ 0x20;
    buf2[2] = FLAG;
    buf2Size=3;
  }
  else{
    buf2[0] = BCC2;
    buf2[1] = FLAG;
  }
  
  // Byte Stuffing (data buffer)
  for (int i = 0, k=0; i<lenght; i++, k++){
    if (buffer[i] == 0x7E || buffer[i] == 0x7D){
      currentLenght++;
      dataBuffer = (unsigned char *) realloc(dataBuffer, currentLenght); 

      dataBuffer[k+1] = buffer[i] ^ 0x20;
      dataBuffer[k] = 0x7D;
      k++;
    }
    else{
      dataBuffer[k] = buffer[i];
    }
  }

  unsigned char finalBuffer[currentLenght + 6]; /*trama I completa*/

  fillFinalBuffer(finalBuffer, buf1, buf2, buf2Size, dataBuffer, currentLenght);

  stateMachineSetUp(C_RR(linkLayer.sequenceNumber^0x01), A_ER, Start, Write);

  if (writeCycle(writeR, fd, finalBuffer, sizeof(finalBuffer)) <0){
    log_error("llwrite() - failed writing");
    return -1;
  }
//  definitely lost: 12,527 bytes in 482 blocks
//  definitely lost: 485 bytes in 241 blocks
  linkLayer.sequenceNumber ^= 0x01;
  free(dataBuffer);
  free(buf2);
  return 0;
}

int llread(int fd, unsigned char *buffer){
  unsigned char *dataBuf;
  int retBufferSize;
  stateMachineSetUp(C_I(linkLayer.sequenceNumber), A_ER, Start, Read);

  unsigned char c;

  if (readingCycle(readR, fd, &c, &dataBuf, &retBufferSize) < 0){
    log_error("llread() - failed reading");
    tcflush(fd, TCIFLUSH);
    return -1;
  }

  unsigned char replyBuf[5] = {FLAG, A_ER, c, BCC(A_ER, c), FLAG};
  
  int res = write(fd,replyBuf,5);
  log_message("SENDING RESPONSE\n");
  for (int i=0; i < 5; i++){
    log_hexa(replyBuf[i]);
  }
  if (res == -1) {
    log_error("llread() - Failed writing response to buffer.");
    return -1;}

  for (int i = 0; i < retBufferSize; i++)
    buffer[i] = dataBuf[i];

  free(dataBuf);
  return retBufferSize; 
}

int llclose(int fd){
  int returnValue = fd;

  switch (linkLayer.status)
  {
  case TRANSMITTER:

    if (transmitter_DISC_UA(fd) <0)
      returnValue = -1;
    break;
  case RECEIVER:
    if (receiver_DISC_UA(fd) <0)
      returnValue = -1;
    break;
  }

  if (tcsetattr(fd,TCSANOW,&oldtio) != 0){
    log_error("llclose() - Error on tcsetattr()");
    return -1;
  }
  if (close(fd) != 0){
    log_error("llclose() - Error on close()");
    return -1;
  }
  if (sigaction(SIGALRM, &old_action, NULL)<0){
    log_error("llclose() - error on sigaction");
    return -1;
  }

  if (returnValue>0)
    log_success("Closing Successful");

  return returnValue;
}
/** @}*/
