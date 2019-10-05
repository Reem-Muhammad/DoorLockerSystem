/*
 * control_ecu.c
 *
 *  Created on: 29 Sep 2019
 *      Author: Home
 */


#include "LCD.h"
#include "UART.h"
#include "EEPROM.h"

#define PASSWORD_LENGTH 5
#define CONFIRMATION_FAILED 0
#define CONFIRMATION_PASSED 1
#define MOTOR_IN1
#define MOTOR_IN2
#define MOTOR_EN1
#define BUZZER


const Uart_ConfigType s_UartConfig = {PARITY_DISABLED, ONE_STOP_BIT, CHAR_8, 9600};
uint8 data_buffer[PASSWORD_LENGTH] = {0};
uint8 confirm_buffer[PASSWORD_LENGTH] = {0};

void saveNewPass()
{
	for(uint8 i = 0; i<PASSWORD_LENGTH; i++)
	{
		data_buffer[i] = UART_receiveByte();
		LCD_displayInt(data_buffer[i]);
	}

	Eep_Write(0, data_buffer, PASSWORD_LENGTH);
	//LCD_displayInt(8888); //debug

}

uint8 confirmPass()
{
	/*receive the confirmation password entered by the user*/

	//LCD_displayString("Ctrl ECU.."); //debug
	for(uint8 i = 0; i<PASSWORD_LENGTH; i++)
	{
		confirm_buffer[i] = UART_receiveByte();
	}

	/*compare it to the saved password*/
	LCD_goToRowCol(0,0);
	//LCD_displayString("                 "); //debug
	//LCD_displayInt(Eep_Read(0, data_buffer, PASSWORD_LENGTH)); //debug
	//LCD_displayString("Reading.."); //debug
	Eep_Read(0, data_buffer, PASSWORD_LENGTH);

	for(uint8 i = 0; i<PASSWORD_LENGTH; i++)
	{
		if (confirm_buffer[i] != data_buffer[i])
		{
			LCD_displayInt(i); //debug
			UART_sendByte(CONFIRMATION_FAILED);
			return CONFIRMATION_FAILED;
		}
	}
	UART_sendByte(CONFIRMATION_PASSED);
	return CONFIRMATION_PASSED;
}

int main()
{
	LCD_init();
	UART_init(&s_UartConfig);
	Eep_init();
	saveNewPass();
	//confirmPass();

	while (confirmPass() == CONFIRMATION_FAILED)
	{
		saveNewPass();
		confirmPass();
	}

	while(1)
	{



	}
}
