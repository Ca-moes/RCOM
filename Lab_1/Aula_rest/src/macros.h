/** \addtogroup Macros
 *  @{
 */
#ifndef MACROS_HEADER
#define MACROS_HEADER

/*-----------Variables-----------*/

#define BAUDRATE              B50
#define TIME_OUT              3
#define ATTEMPTS              5
#define MAX_SIZE              1024 // Tamanho máximo em bytes dos dados da trama I === Tamanho máximo do pacote de dados
// entre 24-inf

#define PROBABILITY_BCC2      0 //percentage
#define PROBABILITY_BCC1      0 //percentage
#define T_PROP_DELAY          0

/**
 * @brief Enum com Valores para a Máquina de Estados de SET-UA
 */

#define SU_FRAME_SIZE         5  ///< tamanho em bytes das tramas de Supervisão e Não Numeradas

#define FLAG                  0b01111110  ///< (0x7E) flag de inicio e fim

#define A_ER                  0b00000011  ///< (0x03) Campo de Endereço (A) de commandos do Emissor, resposta do Receptor
#define A_RE                  0b00000001  ///< (0x01) Campo de Endereço (A) de commandos do Receptor, resposta do Emissor

#define C_SET                 0b00000011 ///< (0x03) Campo de Controlo - SET (set up)
#define C_DISC                0b00001011 ///< (0x0B) Campo de Controlo - DISC (disconnect)
#define C_UA                  0b00000111 ///< (0x07) Campo de Controlo - UA (Unnumbered Acknowledgement)
#define C_RR(r)               ((0b00000101) ^ (r) << (7)) ///< (0x05 OU 0x85) Campo de Controlo - RR (receiver ready / positive ACK))
#define C_REJ(r)              ((0b00000001) ^ (r) << (7)) ///< (0x01 OU 0x81) Campo de Controlo - REJ (reject / negative ACK))
#define C_I(r)                ((0b01000000) & (r) << (6)) ///< (0x00 0x40) Campo de Controlo - Tramas I 

#define BCC(a,c)              (a ^ c) ///< XOR entre a e c


#define FALSE                 0
#define TRUE                  1

// valores de type usados em application.c
#define TRANSMITTER           1
#define RECEIVER              0

/*-------package control--------*/

//C flag
#define DATA                  0x01
#define START                 0x02
#define END                   0x03

//T flag
#define T_SIZE                0x00
#define T_NAME                0x01


#define MAX_SIZE_AFT_STUFF    2*MAX_SIZE
#endif // MACROS_HEADER
/** @}*/
