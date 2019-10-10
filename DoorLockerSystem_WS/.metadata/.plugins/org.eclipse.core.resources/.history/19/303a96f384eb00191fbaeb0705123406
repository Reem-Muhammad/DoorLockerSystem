/*
 * UART.h
 *
 *  Created on: 23 Sep 2019
 *      Author: Home
 */

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "common_macros.h"
#include "std_types.h"

#define ASYNCHRONOUS 0
#define SYNCHRONOUS 1

/*~~~~~~~~ Static Configurations ~~~~~~~~*/
#define UART_MODE ASYNCHRONOUS
#define DOUBLE_SPEED
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#ifdef DOUBLE_SPEED
	#define BAUD_RATE_GENERATOR(BAUD_RATE) ( ( (F_CPU) / ( (8UL) * (BAUD_RATE) ) ) - 1 )
#else
	#define BAUD_RATE_GENERATOR(BAUD_RATE) ( ( (F_CPU) / ( (16UL) * (BAUD_RATE) ) ) - 1 )
#endif



/*~~~~~~~~~~~~~~~ Type Definitions ~~~~~~~~~~~~~~~*/
typedef enum
{
	PARITY_DISABLED, RESERVED, EVEN_PARITY, ODD_PARITY
}Uart_ParityModeType;

typedef enum
{
	ONE_STOP_BIT, TWO_STOP_BITS
}Uart_StopBitType;

typedef enum
{
	CHAR_5_, CHAR_6, CHAR_7, CHAR_8
}Uart_CharSizeType;


typedef struct
{
	Uart_ParityModeType e_Uart_parityMode;
	Uart_StopBitType e_Uart_stopBit;
	Uart_CharSizeType e_Uart_charSize;

	uint16 Uart_baudRate;
}Uart_ConfigType;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~ Functions Prototypes ~~~~~~~~~~~~~*/
void UART_init(const Uart_ConfigType *config_ptr);
void UART_sendByte(uint8 data);
void UART_sendString(uint8 *str_ptr);
uint8 UART_receiveByte(void);
void UART_receiveString(uint8 *str_ptr);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif /* UART_H_ */
