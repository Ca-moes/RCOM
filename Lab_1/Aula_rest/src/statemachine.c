#include "statemachine.h"

int stateMachine(unsigned char byte, unsigned char **buf, int *size){
  switch (state_machine.state)
  {
  case Start:
    return processStart(byte);
    break;
  case FLAG_RCV:
    return processFLAG_RCV(byte);
    break;
  case A_RCV:
    return processA_RCV(byte);
    break;
  case C_RCV:
    return processC_RCV(byte);
    break;
  case BCC_OK:
    return processBCC_OK(byte, buf, size);
    break;
  case DONE:
    return 0;
    break;
  default:
    log_error("stateMachine() - unknown state value");
    return -1;
    break;
  }
  return 0;
}

int processStart(unsigned char byte){
  if (state_machine.type == Read)
  {
    frameIndex = 0;
    wrongC = FALSE;
    if (byte == FLAG){
      state_machine.state = FLAG_RCV;
      frameIndex++;
    }  
  }
  else
  {
    if (byte == FLAG) 
      state_machine.state = FLAG_RCV;  
  }
  return 0;
}

int processFLAG_RCV(unsigned char byte){
  if (byte == state_machine.address) {
      state_machine.state = A_RCV;
      checkBuffer[0] = byte;
      if (state_machine.type == Read)
        frameIndex++;
    }
    else if (byte != FLAG)
      state_machine.state = Start;
  return 0;
}

int processA_RCV(unsigned char byte){
  switch (state_machine.type)
  {
  case Supervision:
    if (byte == state_machine.control) {
      state_machine.state = C_RCV;
      checkBuffer[1] = byte;} 
    else if (byte == FLAG)
      state_machine.state = FLAG_RCV;
    else
      state_machine.state = Start;    
    break;

  case Write:
    if (byte == C_REJ(linkLayer.sequenceNumber^0x01)){
      log_error("stateMachine_Write() - Reject Control Byte Received");
      return -1;
    }

    if (byte == state_machine.control) { // Control esperado
      state_machine.state = C_RCV;
      checkBuffer[1] = byte;} 
    else if (byte == FLAG)
      state_machine.state = FLAG_RCV;
    else{
      state_machine.state = Start;    
    }
    break;

  case Read:
    if (byte == C_I(linkLayer.sequenceNumber ^ 1)) {
      log_caution("stateMachine_Read() - Control Byte with wrong sequence number, need to check BCC");
      wrongC = TRUE;
      state_machine.state = C_RCV;
      checkBuffer[1] = byte;
      frameIndex++;
    }
    else if (byte == state_machine.control) {  // Control esperado
      state_machine.state = C_RCV;
      checkBuffer[1] = byte;
      frameIndex++;
    } else if (byte == FLAG){
      state_machine.state = FLAG_RCV;
      frameIndex = 1;
    } else 
      state_machine.state = Start;
    break;
  default:
    log_error("processA_RCV() - unknown type value");
    return -1;
    break;
  }
  return 0;
}

int processC_RCV(unsigned char byte){
  if (byte == BCC(checkBuffer[0],checkBuffer[1])){
    if (state_machine.type == Read && wrongC == TRUE){
      log_caution("stateMachine_Read() - Received already read Packet");
      return -2;
    }
    state_machine.state = BCC_OK;
    if (state_machine.type == Read)
      frameIndex++;
  }
  else if (byte == FLAG)
  {
    state_machine.state = FLAG_RCV;
    if (state_machine.type == Read)
      frameIndex = 1;
  }
  else
  {
    switch (state_machine.type)
    {
    case Supervision:
      state_machine.state = Start;
      break;
    case Write:
      log_error("stateMachine_Write() - Error in BCC");
      return -1;
      break;
    case Read:
      log_error("stateMachine_Read() - BCC received with Errors");
      return -1;
      break;
    default:
      log_error("processC_RCV() - unknown type value");
      return -1;
      break;
    }
  }
  return 0;
}

int processBCC_OK(unsigned char byte, unsigned char **buffer, int *buffersize){
  if (state_machine.type == Supervision || state_machine.type == Write)
  {
    if (byte == FLAG)
      state_machine.state = DONE;
    else
      state_machine.state = Start;
  }
  else if (state_machine.type == Read)
  {
    frameIndex++;
    if (byte == FLAG){
      *buffer = (unsigned char *)malloc((frameIndex-6));
      *buffersize = 0;

      // Byte De-stuffing
      for (int i = 4; i < frameIndex - 2; i++)
      {
        if (linkLayer.frame[i] != 0x7D)
          (*buffer)[*buffersize] = linkLayer.frame[i];
        else{
          (*buffer)[*buffersize] = linkLayer.frame[i+1] ^ 0x20;
          i++;
        }
        (*buffersize)++;
      }
      *buffer = (unsigned char *) realloc(*buffer, (*buffersize)); 
      
      // Formação de BCC2
      unsigned char BCC2 = (*buffer)[0];
      for (int i = 1; i < *buffersize; i++)
        BCC2 = BCC2 ^ (*buffer)[i];

      if (linkLayer.frame[frameIndex-2]==BCC2){
        linkLayer.sequenceNumber = linkLayer.sequenceNumber ^ 1;
        state_machine.state = DONE;
      }
      else{
        log_error("stateMachine_Read() - BCC2 received with Errors");
        return -1;
      }
    }
  }
  return 0;
}
