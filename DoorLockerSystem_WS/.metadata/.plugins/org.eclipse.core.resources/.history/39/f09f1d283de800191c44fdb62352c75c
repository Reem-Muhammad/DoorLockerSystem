/*
 * hmi_ecu_functions.c
 *
 *  Created on: 6 Oct 2019
 *      Author: Home
 */

#include "hmi_ecu_functions.h"

uint8 alarmOn_flag = 0;
uint8 password_counter = 0; 	/*Keeps track of the number of digits the user has entered so far*/

/*
 * Should it be an 'extern'?? it is not the same variable used by hmi_ecu.c,
 * but no conflict would arise if both files used the same variable
 */
uint8 key_f = 0;			/*Stores the pressed key*/

/*------------------------------------------
 * [Function Name]: alarmStart
 * [Description]: Starts an alarm for the period specified by counterTop and n_ticksRequired
 * [Args]:
* 		counterTop: TOP value to be compared with the counter value.
 * 		n_ticksRequired: Number of ticks required to count the required time, given the specified counterTop.
 * [Return]: None
 -------------------------------------------*/
void alarmStart(uint16 counterTop, uint8 n_ticksRequired)
{
	/*Set the callback function*/
	Ocu_setCbk(alarmStop);

	/*Send a command to the Control ECU to start the alarm*/
	UART_sendByte(START_ALARM);

	alarmOn_flag = 1;
	LCD_clear();
	LCD_goToRowCol(0,7);
	LCD_displayString("INTRUDER ALERT!!! ");



	/*start the timer*/
	Ocu_start(counterTop, n_ticksRequired);


}

/*------------------------------------------
 * [Function Name]: alarmStop
 * [Description]: a callback function that is executed when a specified time period has passed
 * [Args]: None
 * [Return]: None
 -------------------------------------------*/
void alarmStop()
{
	alarmOn_flag = 0;

	/*Send a command to the Control ECU to stop the alarm ========>[BUG#1]*/
	UART_sendByte(STOP_ALARM);

	/*stop the timer*/
	Ocu_stop();
}


/*------------------------------------------
 * [Function Name]: setNewPass
 * [Description]: lets the user set a new password that would be saved in the EEPROM
 * [Args]: None
 * [Return]: None
 -------------------------------------------*/
void setNewPass()
{
	/*Send a command to the Control ECU to start saving the received password*/
	UART_sendByte(SET_NEW_PASS);

	/*Display instructions for the user*/
	LCD_goToRowCol(0,0);
	LCD_displayString("Please Enter New Pass:        ");
	LCD_goToRowCol(1,0);
	LCD_displayString("-----                  ");

	/*initialize the digits counter*/
	password_counter = 0;

	/*move the cursor to the beginning of the second row, where the password is to be displayed*/
	LCD_goToRowCol(1,0);

	/*Enter 5 digits, and transmit them to the Control ECU*/
	while(password_counter < PASSWORD_LENGTH)
	{
		key_f = keypad_getPressedKey();

		/*Send the digit to the Control ECU*/
		UART_sendByte(key_f);
		/*delay to prevent digit replication*/
		_delay_ms(400);

		/*Display '*' instead of the actual digit*/
		LCD_displayCharacter('*');

		password_counter++;
	}

	/*Display instructions for the user*/
	LCD_goToRowCol(0,0);
	LCD_displayString("Press 'C' To Enter.           ");

	/*wait until the user enters 'C'*/
	key_f = keypad_getPressedKey();
	while(key_f != 'C')
	{
		key_f = keypad_getPressedKey();

	}
	_delay_ms(400); /*if not introduced, the first digit in the confirmation password would be 'C'*/
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
	/*Send a command to the Control ECU to start password confirmation*/
	UART_sendByte(CONFIRM_PASS);

	/*Display instructions for the user*/
	LCD_goToRowCol(0,0);
	LCD_displayString("Please Confirm The Password:           ");
	LCD_goToRowCol(1,0);
	LCD_displayString("-----                     ");

	/*Initialize the digits counter*/
	password_counter = 0;

	/*move the cursor to the second row, where the password is to be displayed*/
	LCD_goToRowCol(1,0);

	/*Send the confirmation password to the control ECU*/
	while(password_counter < PASSWORD_LENGTH)
	{
		key_f = keypad_getPressedKey();
		UART_sendByte(key_f);

		/*delay to prevent digit replication*/
		_delay_ms(400);

		/*Display '*' instead of the actual digit*/
		LCD_displayCharacter('*');

		password_counter++;
	}

	/*Display instructions for the user*/
	LCD_goToRowCol(0,0);
	LCD_displayString("Press 'C' To Enter.         ");

	/*wait until the user enters 'C'*/
	key_f = keypad_getPressedKey();
	while(key_f != 'C')
	{
		key_f = keypad_getPressedKey();
	}

	_delay_ms(400); /*if not introduced and there was a mismatch, the first digit of the new password would be 'C'*/
	//uint8 confirm = UART_receiveByte(); //[SOLVED] doesn't return --> reason: Eep_Write() wasn't returning due to the polling in TWI_stop()

	return UART_receiveByte();
}
