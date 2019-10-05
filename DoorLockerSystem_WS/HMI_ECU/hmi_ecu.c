/*
 * hmi_ecu.c
 *
 *  Created on: 29 Sep 2019
 *      Author: Home
 */


#include "keypad.h"
#include "LCD.h"
#include "UART.h"
#include "Ocu.h"

#define PASSWORD_LENGTH 5
#define CONFIRMATION_FAILED 0
#define CONFIRMATION_PASSED 1
#define WRONG_PASS_MAX 3

#define SET_NEW_PASS 0xF0
#define CONFIRM_PASS 0x0F
#define START_ALARM 0x8A
#define STOP_ALARM 0xA8
#define OPEN_DOOR 0xDB
#define CLOSE_DOOR 0xBD




uint8 alarmOn_flag = 0;
uint8 thief_flag = 0;

uint8 password_counter = 0; 	/*Keeps track of the number of digits the user has entered so far*/
uint8 key = 0;		/*Stores the pressed key*/
uint8 wrongPassCounter = 0;		/*keeps track of the number of wrong attempts to enter the password*/

const Uart_ConfigType s_UartConfig = {PARITY_DISABLED, ONE_STOP_BIT, CHAR_8, 9600}; /*Configuration set of the UART*/
const Ocu_ConfigType s_OcuConfig = {OCU_DISABLE,OCU_PRESCALER_1024};

void alarmStart()
{
	alarmOn_flag = 1;
	LCD_clear();
	LCD_goToRowCol(0,7);
	LCD_displayString("INTRUDER ALERT!!! ");

	UART_sendByte(START_ALARM);

	/*start the timer*/
	Ocu_start(46874, 10);


}
void alarmStop()
{
	alarmOn_flag = 0;
	//LCD_goToRowCol(0,0);
	//LCD_displayString("Alarm Stopped            ");
	_delay_ms(100);

	UART_sendByte(STOP_ALARM);
	/*terminate the timer*/
	Ocu_stop();
}

/*------------------------------------------
 * [Function Name]: setNewPass
 * [Description]: lets the user to set a new password to be saved in the EEPROM
 * [Args]: None
 * [Return]: None
 -------------------------------------------*/
void setNewPass()
{
	UART_sendByte(SET_NEW_PASS);
	/*Display instructions for the user*/
	LCD_goToRowCol(0,0);
	LCD_displayString("Please Enter New Pass:        ");
	LCD_goToRowCol(1,0);
	LCD_displayString("-----                  ");

	password_counter = 0;
	LCD_goToRowCol(1,0);

	/*Enter 5 digits, and transmit them to the control ECU*/
	while(password_counter < PASSWORD_LENGTH)
	{
		key = keypad_getPressedKey();
		UART_sendByte(key);
		_delay_ms(400);

		/*Display '*' instead of the actual digit*/
		LCD_displayCharacter('*');

		password_counter++;
	}

	/*Display instructions for the user*/
	LCD_goToRowCol(0,0);
	LCD_displayString("Press 'C' To Enter.           ");

	/*wait until the user enters 'C'*/
	key = keypad_getPressedKey();
	while(key != 'C')
	{
		key = keypad_getPressedKey();

	}
	_delay_ms(400); //if not introduced, the first digit in the confirmation password would be 'C'
}



/*------------------------------------------
 * [Function Name]: confirmPass
 * [Description]: Sends the confirmation password to the control ECU to be compared with the saved one
 * [Args]: None
 * [Return]:
		CONFIRMATION_FAILED for password mismatch
		CONFIRMATION_PASS for password match
 -------------------------------------------*/
uint8 confirmPass()
{
	UART_sendByte(CONFIRM_PASS);
	/*Display instructions for the user*/
	LCD_goToRowCol(0,0);
	LCD_displayString("Please Confirm The Password:           ");
	LCD_goToRowCol(1,0);
	LCD_displayString("-----                     ");

	password_counter = 0;
	LCD_goToRowCol(1,0);

	/*Send the confirmation password to the control ECU*/
	while(password_counter < PASSWORD_LENGTH)
	{
		key = keypad_getPressedKey();
		UART_sendByte(key);
		_delay_ms(400);

		/*Display '*' instead of the actual digit*/
		LCD_displayCharacter('*');
		password_counter++;
	}

	/*Display instructions for the user*/
	LCD_goToRowCol(0,0);
	LCD_displayString("Press 'C' To Enter.         ");

	/*wait until the user enters 'C'*/
	key = keypad_getPressedKey();
	while(key != 'C')
	{
		key = keypad_getPressedKey();
	}

	_delay_ms(400); //if not introduced and there was a mismatch, the first digit of the new password would be 'C'
	//uint8 confirm = UART_receiveByte(); //[SOLVED] doesn't return --> reason: Eep_Write() wasn't returning due to the polling in TWI_stop()

	return UART_receiveByte();
}



int main()
{
	LCD_init();
	UART_init(&s_UartConfig);
	SET_BIT(SREG, 7);
	Ocu_init(&s_OcuConfig);
	Ocu_setCbk(alarmStop);


	setNewPass();


	/*Reset the password and confirm again as long as there is password mismatch*/
	while (confirmPass() == CONFIRMATION_FAILED)
	{
		LCD_goToRowCol(0,0);
		LCD_displayString("Confirmation Failed           ");
		_delay_ms(500);

		setNewPass();
		_delay_ms(10);
	}

	LCD_goToRowCol(0,0);
	LCD_displayString("Password Confirmed        ");
	_delay_ms(500);


	while(1)
	{

		LCD_goToRowCol(0,0);
		LCD_displayString("'+' : Open Door            ");
		LCD_goToRowCol(1,0);
		LCD_displayString("'-' : Change Password      ");

		key = keypad_getPressedKey();
		while(key != '+' && key != '-')
		{
			key = keypad_getPressedKey();
			_delay_ms(300);
		}

		UART_sendByte(key);
		if(key == '+')
		{
			thief_flag = 0;
			wrongPassCounter = 0;
			while (confirmPass() == CONFIRMATION_FAILED)
			{
				wrongPassCounter++;
				if(wrongPassCounter == WRONG_PASS_MAX)
				{
					//UART_sendByte('A');
					alarmStart();
					thief_flag = 1;
					while(alarmOn_flag);
					break;
				}
			}
			if(thief_flag == 0)
			{
				UART_sendByte(OPEN_DOOR);
				LCD_clear();
				LCD_goToRowCol(0,0);
				LCD_displayString("Openning ...        ");
				_delay_ms(15000);

				UART_sendByte(CLOSE_DOOR);
				LCD_goToRowCol(0,0);
				LCD_displayString("Closing ...         ");
				_delay_ms(15000);
			}
		}

		else if(key == '-')
		{
			if (confirmPass() == CONFIRMATION_PASSED)
			{
				setNewPass();

				/*Reset the password and confirm again as long as there is password mismatch*/
				while (confirmPass() == CONFIRMATION_FAILED)
				{
					LCD_goToRowCol(0,0);
					LCD_displayString("Confirmation Failed           ");
					_delay_ms(500);

					setNewPass();
					_delay_ms(10);
				}

				LCD_goToRowCol(0,0);
				LCD_displayString("Password Changed Successfully        ");
				_delay_ms(500);

			}
		}

	}
}
