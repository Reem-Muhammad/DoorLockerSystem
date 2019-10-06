/*
 * control_ecu.c
 *
 *  Created on: 29 Sep 2019
 *      Author: Home
 */


#include "LCD.h"
#include "UART.h"
#include "EEPROM.h"
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

#define MOTOR_DRIVER_DIR DDRD
#define MOTOR_DRIVER_PORT PORTD
#define MOTOR_IN1 PD5
#define MOTOR_IN2 PD6
#define MOTOR_EN1 PD7
#define BUZZER_DIR DDRC
#define BUZZER_PORT PORTC
#define BUZZER PC7


const Uart_ConfigType s_UartConfig = {PARITY_DISABLED, ONE_STOP_BIT, CHAR_8, 9600};
const Ocu_ConfigType s_OcuConfig = {OCU_PIN_DISABLE,OCU_PRESCALER_1024};
uint8 data_buffer[PASSWORD_LENGTH] = {0};
uint8 confirm_buffer[PASSWORD_LENGTH] = {0};
//uint8 key = 0;
//uint8 wrongPassCounter = 0;
uint8 commandReceived = 0;


void doorOpen()
{
	/*rotate the motor CW*/
	SET_BIT(MOTOR_DRIVER_PORT, MOTOR_IN1);
	CLEAR_BIT(MOTOR_DRIVER_PORT, MOTOR_IN2);
	SET_BIT(MOTOR_DRIVER_PORT, MOTOR_EN1);
	_delay_ms(15000);
	/*stop the motor*/
	CLEAR_BIT(MOTOR_DRIVER_PORT, MOTOR_EN1);

}
void doorClose()
{
	/*rotate the motor CCW*/
	CLEAR_BIT(MOTOR_DRIVER_PORT, MOTOR_IN1);
	SET_BIT(MOTOR_DRIVER_PORT, MOTOR_IN2);
	SET_BIT(MOTOR_DRIVER_PORT, MOTOR_EN1);
	_delay_ms(15000);
	/*stop the motor*/
	CLEAR_BIT(MOTOR_DRIVER_PORT, MOTOR_EN1);
}
void saveNewPass()
{
	LCD_goToRowCol(0,0);
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
	LCD_goToRowCol(1,0);
	//LCD_displayString("Ctrl ECU.."); //debug
	for(uint8 i = 0; i<PASSWORD_LENGTH; i++)
	{
		confirm_buffer[i] = UART_receiveByte();
		LCD_displayInt(confirm_buffer[i]);
	}

	/*compare it to the saved password*/
	//LCD_goToRowCol(0,0);
	//LCD_displayString("                 "); //debug
	//LCD_displayInt(Eep_Read(0, data_buffer, PASSWORD_LENGTH)); //debug
	//LCD_displayString("Reading.."); //debug
	Eep_Read(0, data_buffer, PASSWORD_LENGTH);

	for(uint8 i = 0; i<PASSWORD_LENGTH; i++)
	{
		if (confirm_buffer[i] != data_buffer[i])
		{
			//LCD_displayInt(i); //debug
			UART_sendByte(CONFIRMATION_FAILED);
			return CONFIRMATION_FAILED;
		}
	}
	UART_sendByte(CONFIRMATION_PASSED);
	return CONFIRMATION_PASSED;
}


void alarmStart()
{
	CLEAR_BIT(BUZZER_PORT, BUZZER);
	LCD_displayString("ALARM");

	/*start the timer*/
	//Ocu_start(46874, 10);
}

void alarmStop()
{
	SET_BIT(BUZZER_PORT, BUZZER);
	LCD_displayString("STOPPED");

	/*terminate the timer*/
	Ocu_stop();
}
int main()
{
	LCD_init();
	UART_init(&s_UartConfig);
	SET_BIT(SREG, 7);
	Ocu_init(&s_OcuConfig);
	Ocu_setCbk(alarmStop);
	Eep_init();

	SET_BIT(BUZZER_DIR, BUZZER);
	SET_BIT(BUZZER_PORT, BUZZER);		/*init*/
	MOTOR_DRIVER_DIR |= (1<< MOTOR_IN1) | (1<< MOTOR_IN2) | (1<< MOTOR_EN1);

#if 0
	commandReceived = UART_receiveByte();
	if(commandReceived == SET_NEW_PASS)
	{
		//LCD_goToRowCol(1,0);
		//LCD_displayString("SET_NEW_PASS");
		saveNewPass();
	}

	//confirmPass();

	while (confirmPass() == CONFIRMATION_FAILED)
	{
		saveNewPass();
		//confirmPass();
	}

	while(1)
	{
		key = UART_receiveByte();
		if(key == '+')
		{
			while (confirmPass() == CONFIRMATION_FAILED)
			{
#if 1
				wrongPassCounter++;
				if(wrongPassCounter == WRONG_PASS_MAX)
				{
					if(UART_receiveByte() == 'A')
					{
						alarmStart();
						while(1);
					}

				}
#endif
			}
		}


	}
#endif
	//LCD_displayInt(8);
	while(1)
	{
		commandReceived = UART_receiveByte();
		switch(commandReceived)
		{
		case SET_NEW_PASS:
			saveNewPass();
			break;
		case CONFIRM_PASS:
			confirmPass();
			break;
		case START_ALARM:
			alarmStart();
			break;
		case STOP_ALARM:
			alarmStop();
			break;
		case OPEN_DOOR:
			doorOpen();
			break;
		case CLOSE_DOOR:
			doorClose();
			break;
		default:
			break;
		}

	}
}
