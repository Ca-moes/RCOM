// A_RCV

/**
*   Mudar controlos esperados de write e read
*/
// Supervision
    if (byte == c_flag) {
      state = C_RCV;
      checkBuffer[1] = byte;} 
    else if (byte == FLAG)
      state = FLAG_RCV;
    else
      state = Start;    
    break;

// write
    if (byte == C_REJ(linkLayer.sequenceNumber^0x01)){
      log_error("stateMachine_Write() - Reject Control Byte Received");
      return -1;
    }

    if (byte == C_RR(linkLayer.sequenceNumber^0x01)) { // Control esperado
      state = C_RCV;
      checkBuffer[1] = byte;} 
    else if (byte == FLAG)
      state = FLAG_RCV;
    else{
      state = Start;    
    }
    break;

// read
    if (byte == C_I(linkLayer.sequenceNumber ^ 1)) {
      log_caution("stateMachine_Read() - Control Byte with wrong sequence number, need to check BCC");
      wrongC = TRUE;
      state = C_RCV;
      checkBuffer[1] = byte;
      frameIndex++;
    }
    else if (byte == C_I(linkLayer.sequenceNumber)) {  // Control esperado
      state = C_RCV;
      checkBuffer[1] = byte;
      frameIndex++;
    } else if (byte == FLAG){
      state = FLAG_RCV;
      frameIndex = 1;
    } else 
      state = Start;
    break;

// C_RCV
// Supervision
    if (byte == BCC(checkBuffer[0],checkBuffer[1]))
      state = BCC_OK;
    else if (byte == FLAG)
      state = FLAG_RCV;
    else
      state = Start;
    break;

// write
    if (byte == BCC(checkBuffer[0],checkBuffer[1]))
      state = BCC_OK;
    else if (byte == FLAG)
      state = FLAG_RCV;
    else{
      log_error("stateMachine_Write() - Error in BCC");
      return -1; }
    break;

// read
    if (byte == BCC(checkBuffer[0],checkBuffer[1])){
      if (wrongC == TRUE){
        log_caution("stateMachine_Read() - Received already read Packet");
        return -2;
      }
      state = BCC_OK;
      frameIndex++;
    }
    else if (byte == FLAG){
      state = FLAG_RCV;
      frameIndex = 1;
    }
    else{
      log_error("stateMachine_Read() - BCC received with Errors");
      return -1;
    }
    break;

// BCC_OK
// Supervision
    if (byte == FLAG)
      state = DONE;
    else
      state = Start;
    break;
// Write
    if (byte == FLAG)
      state = DONE;
    else
      state = Start;
    break;
// Read
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
        state = DONE;
      }
      else{
        log_error("stateMachine_Read() - BCC2 received with Errors");
        return -1;
      }
    }
    break;
    