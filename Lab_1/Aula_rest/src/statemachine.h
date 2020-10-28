#ifndef SM_HEADER
#define SM_HEADER

#include "macros.h"
#include "logs.h"
#include "dcp.h"

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
 * @brief The StateMachine function that processes a byte and updates it's state
 * 
 * @param byte byte to process
 * @param buf buffer to put data if stateMachineType is Read
 * @param size variable to keeo size of buf if stateMachineType is Read
 * @return int negative value in case of error, 0 otherwise
 */
int stateMachine(unsigned char byte, unsigned char **buf, int *size);

/**
 * @brief Function to process a byte in case the state is Start
 * 
 * @param byte byte to be processed
 * @return int negative value in case of error, 0 otherwise
 */
int processStart(unsigned char byte);
/**
 * @brief Function to process a byte in case the state is FLAG_RCV
 * 
 * @param byte byte to be processed
 * @return int negative value in case of error, 0 otherwise
 */
int processFLAG_RCV(unsigned char byte);
/**
 * @brief Function to process a byte in case the state is A_RCV
 * 
 * @param byte byte to be processed
 * @return int negative value in case of error, 0 otherwise
 */
int processA_RCV(unsigned char byte);
/**
 * @brief Function to process a byte in case the state is C_RCV
 * 
 * @param byte byte to be processed
 * @return int negative value in case of error, 0 otherwise
 */
int processC_RCV(unsigned char byte);
/**
 * @brief Function to process a byte in case the state is BCC_OK
 * 
 * @param byte byte to be processed
 * @param buffer buffer to put data if stateMachineType is Read
 * @param buffersize variable to keeo size of buf if stateMachineType is Read
 * @return int negative value in case of error, 0 otherwise
 */
int processBCC_OK(unsigned char byte, unsigned char **buffer, int *buffersize);

#endif // PLA_HEADER
