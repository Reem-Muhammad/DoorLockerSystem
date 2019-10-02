/*
 * hmi_ecu.c
 *
 *  Created on: 29 Sep 2019
 *      Author: Home
 */


#include "keypad.h"
#include "LCD.h"
#include "UART.h"

#define PASSWORD_LENGTH 5


uint8 password_counter = 0;
uint8 key = 0;
const Uart_ConfigType s_UartConfig = {PARITY_DISABLED, ONE_STOP_BIT, CHAR_8, 9600};

void setNewPass()
{
	LCD_goToRowCol(0,0);
	LCD_displayString("Please Enter New Pass:");
	LCD_goToRowCol(1,0);
	LCD_displayString("-----");

	LCD_goToRowCol(1,0);
	while(password_counter < PASSWORD_LENGTH)
	{
		key = keypad_getPressedKey();
		UART_sendByte(key);
		_delay_ms(400);
		LCD_displayCharacter('*');
		password_counter++;
	}
	LCD_goToRowCol(0,0);
	LCD_displayString("Press '#' To Enter.         ");
	key = keypad_getPressedKey();
	while(key != '#')
	{
		key = keypad_getPressedKey();
	}
	//LCD_clear();
}


/*Password Confirmation*/
void confirmPass()
{

}

int main()
{
	LCD_init();
	UART_init(&s_UartConfig);

	setNewPass();
	confirmPass();

	while(1)
	{


	}
}
