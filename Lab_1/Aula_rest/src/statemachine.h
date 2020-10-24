#ifndef SM_HEADER
#define SM_HEADER

#include "macros.h"
#include "logs.h"
#include "pla.h"

enum stateMachineType {Supervision, Write, Read};
enum stateMachineState {Start, FLAG_RCV, A_RCV, C_RCV, BCC_OK, DONE};

struct stateMachineParams {
  unsigned char control; // Control Byte that is suppossed to be received
  unsigned char address; // Address Byte that is suppossed to be received
  enum stateMachineState state; 
  enum stateMachineType type;
};

struct stateMachineParams state_machine;
unsigned checkBuffer[2];
int frameIndex, wrongC;


void stateMachineSetUp(unsigned char control, unsigned char address, enum stateMachineState state, enum stateMachineType type);
int stateMachine(unsigned char byte, unsigned char **buf, int *size);

int processStart(unsigned char byte);
int processFLAG_RCV(unsigned char byte);
int processA_RCV(unsigned char byte);
int processC_RCV(unsigned char byte);
int processBCC_OK(unsigned char byte, unsigned char **buffer, int *buffersize);

#endif // PLA_HEADER
