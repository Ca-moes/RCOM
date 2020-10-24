// transmitter_SET
do{
  attempt++;
  res = write(fd,buf,SU_FRAME_SIZE);
  if (res == -1) {
    log_error("transmitter_SET() - Failed writing SET to buffer.");
    return -1;
  }
      
  alarm(linkLayer.timeout);
  failed = FALSE;
  state_machine.state = Start; 

  while (STOP==FALSE) {       /* loop for input */
    res = read(fd,buf_read,1);   /* returns after 1 char has been input */

    if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
      log_caution("transmitter_SET - Failed reading UA from receiver.");
      if (attempt < linkLayer.numTransmissions) {
        log_caution("Trying again...");
        failed = TRUE;
      }
      break;
    } else if (res == -1){
      log_error("transmitter_SET() - Failed reading UA from buffer.");
      return -1;
    }
    
    stateMachine(buf_read[0], NULL, NULL);
    if (state_machine.state == DONE || failed) STOP=TRUE;
  }
}while (attempt < linkLayer.numTransmissions && failed);
alarm(0);


//llwrite
do{
    attempt++;
    res = write(fd,finalBuffer,sizeof(finalBuffer));
    if (res == -1) {
      log_error("llwrite() - Failed writing data to buffer.");
      return -1;
    }
    
    alarm(linkLayer.timeout);
    failed = FALSE;
    state_machine.state = Start;

    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf_read,1);   /* returns after 1 char has been input */

      if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
        log_caution("llwrite: failed reading RR from receiver.");
        if (attempt < linkLayer.numTransmissions) {
          log_caution("Trying again...");
          failed = TRUE;
        }
        break;
      }
      else if (res == -1){
        log_error("llwrite() - Failed reading RR from buffer.");
        return -1;
      }

      if (stateMachine(buf_read[0], NULL, NULL) < 0){
        log_error("llwrite() - Error while receiving RR or REJ");
        failed = TRUE;
        alarm(0);
        break;
      }

      if (state_machine.state == DONE || failed) STOP=TRUE;
    }
  }while (attempt < linkLayer.numTransmissions && failed);
alarm(0);


//transmitter_DISC_UA
do{
    attempt++;
    res = write(fd,buf,SU_FRAME_SIZE);
    if (res == -1) {
      log_error("transmitter_DISC_UA() - Failed writing DISC to buffer.");
      return -1;
    }
        
    alarm(linkLayer.timeout);
    failed = FALSE;
    state_machine.state = Start; 

    while (STOP==FALSE) {       /* loop for input */
      res = read(fd,buf_read,1);   /* returns after 1 char has been input */

      if (res == -1 && errno == EINTR) {  /*returns -1 when interrupted by SIGALRM and sets errno to EINTR*/
        log_caution("transmitter_DISC_UA() - Failed reading DISC from receiver.");
        if (attempt < linkLayer.numTransmissions) {
          log_caution("Trying again...");
        }
        break;
      } else if (res == -1){
        log_error("transmitter_DISC_UA() - Failed reading DISC from buffer.");
        return -1;
      }
      
      stateMachine(buf_read[0],NULL, NULL);
      
      if (state_machine.state == DONE || failed) STOP=TRUE;
    }
  }while (attempt < linkLayer.numTransmissions && failed);

  alarm(0); // cancel pending alarms




  -------------------

int writeCycle(){
  do{
  attempt++;
  res = write(fd,buf,sizeof(buf));
  if (res == -1) {
    log_error("transmitter_SET() - Failed writing SET to buffer.");
    return -1;
  }

  alarm(linkLayer.timeout);
  failed = FALSE;
  state_machine.state = Start; 
  unsigned char buf[1];
  while (STOP==FALSE) { 
    res = read(fd,buf,1);   /* returns after 1 char has been input */

    if (state_machine.state == DONE || failed) STOP=TRUE;
  }

  }while (attempt < linkLayer.numTransmissions && failed);
  alarm(0);
}

