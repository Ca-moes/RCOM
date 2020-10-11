enum stateMachine {Start, FLAG_RCV, A_RCV, C_RCV, BCC_OK, DONE};

#define ATTEMPT_NUM 3

// Tramas I (informação)
#define SET_SIZE 5  // tamanho em bytes da trama SET
#define UA_SIZE 5  // tamanho em bytes da trama UA


#define FLAG 0b01111110  // (0x7E) flag de inicio e fim

#define A_ER 0b00000011  // (0x03) Campo de Endereço (A) de commandos do Emissor, resposta do Receptor
#define A_RE 0b00000001  // (0x01) Campo de Endereço (A) de commandos do Receptor, resposta do Emissor

#define C_SET 0b00000011 // (0x03) Campo de Controlo - SET (set up)
#define C_DISC 0b00001011 // (0x0B) Campo de Controlo - DISC (disconnect)
#define C_UA 0b00000111 // (0x07) Campo de Controlo - UA (Unnumbered Acknowledgement)
#define C_RR(r) ((0b10000101) &= (r) << (7)) // (0x05 OU 0x85) Campo de Controlo - RR (receiver ready / positive ACK))
#define C_REJ(r) ((0b10000001) &= (r) << (7)) // (0x01 OU 0x81) Campo de Controlo - REJ (reject / negative ACK))

#define BCC(a,c) (a ^ c)