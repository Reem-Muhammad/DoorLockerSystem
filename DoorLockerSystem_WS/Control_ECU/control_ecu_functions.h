/*
 *
 *	Name		: control_ecu_functions.h
 *	Author		: Reem Muhammad
 *	Description	: Header file for the main application (Control ECU)
 *  Created on	: 6 Oct 2019
 *
 */


#ifndef CONTROL_ECU_FUNCTIONS_H_
#define CONTROL_ECU_FUNCTIONS_H_

#include "LCD.h"
#include "UART.h"
#include "EEPROM.h"
#include "Ocu.h"


#define PASSWORD_LENGTH 5
#define CONFIRMATION_FAILED 0
#define CONFIRMATION_PASSED 1
#define WRONG_PASS_MAX 3

extern uint8 data_buffer[PASSWORD_LENGTH];
extern uint8 confirm_buffer[PASSWORD_LENGTH];


/*~~~~~~~~~~~~ Timer settings ~~~~~~~~~*/
#define TOP	46874
#define N_TICKS_REQUIRED 10
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~ Communication Commands ~~~~~~~~~~~~*/
#define SET_NEW_PASS 0xF0
#define CONFIRM_PASS 0x0F
#define START_ALARM 0x8A
#define OPEN_DOOR 0xDB
#define CLOSE_DOOR 0xBD
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#define MOTOR_DRIVER_DIR DDRD
#define MOTOR_DRIVER_PORT PORTD
#define MOTOR_IN1 PD5
#define MOTOR_IN2 PD6
#define MOTOR_EN1 PD7
#define BUZZER_DIR DDRC
#define BUZZER_PORT PORTC
#define BUZZER PC7


/*~~~~~~~~~~~~~~~ Functions Prototypes ~~~~~~~~~~~~~~~~~~~*/
void doorOpen();
void doorClose();
void saveNewPass();
void confirmPass();
void alarmStart(uint16 counterTop, uint8 n_ticksRequired);
void alarmStop();
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#endif /* CONTROL_ECU_FUNCTIONS_H_ */
