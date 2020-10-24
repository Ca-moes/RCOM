#ifndef SM_HEADER
#define SM_HEADER

#include "macros.h"
#include "logs.h"
#include "pla.h"

enum stateMachineType {Supervision, Write, Read};   // Type of State Machine
enum stateMachineState {Start, FLAG_RCV, A_RCV, C_RCV, BCC_OK, DONE};  // State of the State Machine

/**
 * @brief List of Parameters to Personalize the State Machine
 */
struct stateMachineParams {
  unsigned char control; // Control Byte that is suppossed to be received
  unsigned char address; // Address Byte that is suppossed to be received
  enum stateMachineState state; 
  enum stateMachineType type;
};

struct stateMachineParams state_machine; // Instance of the stateMachineParams struct
unsigned checkBuffer[2]; // Buffer to hold Address and Control Bytes to check BCC1
int frameIndex, wrongC;

/**
 * @brief Function to easily change the State Machine
 * 
 * @param control Control Byte that is suppossed to be received
 * @param address Address Byte that is suppossed to be received
 * @param state State of the State Machine
 * @param type Type of State Machine
 */
void stateMachineSetUp(unsigned char control, unsigned char address, enum stateMachineState state, enum stateMachineType type);
/**
 * @brief 
 * 
 * @param byte 
 * @param buf 
 * @param size 
 * @return int 
 */
int stateMachine(unsigned char byte, unsigned char **buf, int *size);

int processStart(unsigned char byte);
int processFLAG_RCV(unsigned char byte);
int processA_RCV(unsigned char byte);
int processC_RCV(unsigned char byte);
int processBCC_OK(unsigned char byte, unsigned char **buffer, int *buffersize);

#endif // PLA_HEADER
