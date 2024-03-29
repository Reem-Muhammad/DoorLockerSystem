/*
 * keypad.h
 *
 *  Created on: 9 Sep 2019
 *      Author: Home
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"



/*~~~~~~~~~~~ static configurations ~~~~~~~~~~~~~~*/
#define N_COL 4
#define N_ROW 4
#define N_BUTTONS (N_COL * N_ROW)
#define KEYPAD_PORT_DIR DDRB
#define KEYPAD_PORT_OUT PORTB
#define KEYPAD_PORT_IN PINB


/*-------------------------------------------------
 * [Macro Identifier]: KEYPAD_DEACTIVATE
 * [DESCRIPTION]: configures the whole port as input port to deactivate the keypad
 --------------------------------------------------*/
#define KEYPAD_DEACTIVATE (KEYPAD_PORT_DIR = 0x00)


/*~~~~~~~~~~~~~~~ Functions Prototypes ~~~~~~~~~~~~~~*/
uint8 keypad_getPressedKey(void);


#endif /* KEYPAD_H_ */
