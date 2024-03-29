/*
 * keypad.c
 *
 *  Created on: 9 Sep 2019
 *      Author: Home
 */


#include "keypad.h"


/*----------------------------------------
 * [Function Name]: keypad_getPressedKey
 * [DESCRIPTION]:
 * [Args]: -
 * [Return]: returns the number/character associated with the pressed key
 -----------------------------------------*/
uint8 keypad_getPressedKey()
{
	uint8 col = 0;
	uint8 row = 0;

#if(N_COL == 4)
	const uint8 keypad_buttons [N_BUTTONS] = {7,8,9,'/',4,5,6,'*',1,2,3,'-','C',0,'=','+'};
#elif (N_COL == 3)
	const uint8 keypad_buttons [N_BUTTONS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, '*', 0, '#'};
#endif
	while (1)
	{
		for (col=0 ; col <N_COL; col++)
		{
			/*all row pins are always inputs.
			 * only one column is configured as o/p each iteration.
			 */
			KEYPAD_PORT_DIR = (0b00010000<<col);

			/*all row pins are connected to internal pull-up resistors.
			 * the selected column is enabled.
			 */
			KEYPAD_PORT_OUT = (~(0b00010000<<col));


			for (row=0; row < N_ROW; row++)
			{
				if (BIT_IS_CLEAR(KEYPAD_PORT_IN, row ))
				{
					return ( keypad_buttons[row*N_COL + col] );
				}

			}
		}
	}
}
