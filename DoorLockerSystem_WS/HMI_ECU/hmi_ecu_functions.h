/*
 *
 *	Name		: hmi_ecu_functions.h
 *	Author		: Reem Muhammad
 *	Description	: Header file for the main application (HMI ECU)
 *  Created on	: 6 Oct 2019
 *
 */

#ifndef HMI_ECU_FUNCTIONS_H_
#define HMI_ECU_FUNCTIONS_H_

#include "keypad.h"
#include "LCD.h"
#include "UART.h"
#include "Ocu.h"

extern uint8 alarmOn_flag;


#define PASSWORD_LENGTH 5
#define CONFIRMATION_FAILED 0
#define CONFIRMATION_PASSED 1
#define WRONG_PASS_MAX 3

/*~~~~~~~~~~ Communication Commands ~~~~~~~~~~~~*/
#define SET_NEW_PASS 0xF0
#define CONFIRM_PASS 0x0F
#define START_ALARM 0x8A
#define OPEN_DOOR 0xDB
#define CLOSE_DOOR 0xBD
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~ Functions Prototypes ~~~~~~~~~~~~~~~~~~~*/
void setNewPass();
uint8 confirmPass();
void alarmStart(Ocu_ValueType Threshold);
void alarmStop();
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif /* HMI_ECU_FUNCTIONS_H_ */
