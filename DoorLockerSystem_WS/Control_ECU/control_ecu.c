/*
 * control_ecu.c
 *
 *  Created on: 29 Sep 2019
 *      Author: Home
 */


#include "LCD.h"
#include "UART.h"

#define PASSWORD_LENGTH 5

const Uart_ConfigType s_UartConfig = {PARITY_DISABLED, ONE_STOP_BIT, CHAR_8, 9600};
uint8 received_byte = 0;
uint8 data_buffer[PASSWORD_LENGTH] = {0};

int main()
{
	LCD_init();
	UART_init(&s_UartConfig);

	while(1)
	{
		/**/
		for(uint8 i = 0; i<PASSWORD_LENGTH; i++)
		{
			data_buffer[i] = UART_receiveByte();
			LCD_displayInt(data_buffer[i]);
		}

	}
}
