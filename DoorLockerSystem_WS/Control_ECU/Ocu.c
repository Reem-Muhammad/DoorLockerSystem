/*
 * Ocu.c
 *
 *  Created on: 4 Oct 2019
 *      Author: Home
 */

#include "Ocu.h"
#include "LCD.h"

uint8 g_n_ticksRequired = 0;
uint8 g_ticksCounter = 0;

void (*g_Ocu_cbkPtr)() = NULL_PTR;

void Ocu_setCbk( void (*cbkPtr)(void) )
{
	g_Ocu_cbkPtr = cbkPtr;
}

/*-------------------------------------------------------------
 * [Function Name]: Ocu_init
 * [Description]: initializes the output compare unit
 * [Args]:
 * 		ConfigPtr: pointer to the configuration set
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_init(const Ocu_ConfigType *ConfigPtr)
{
	/*Set FOC1 for non-PWM mode */
	SET_BIT(TCCR1A, FOC1A);
	//SET_BIT(TCCR1A, FOC1B);

	/*set OC1A behavior on compare match*/
	TCCR1A = (TCCR1A & 0x3F) | (ConfigPtr->e_ocu_pinAction << 6);
	//TCCR1A = (TCCR1A & 0xCF) | (ConfigPtr->e_ocu_pinAction << 4);	/*set OC1B behavior*/

	/*set the source for max counter value to be OCR1A register: WGM13:0 -> 0 1 0 0 */
	TCCR1B |= (1<<WGM12);

	/*set the prescaler*/
	TCCR1B = (TCCR1B & 0xF8) | (ConfigPtr->e_ocu_prescaler);
}

/*-------------------------------------------------------------
 * [Function Name]: Ocu_start
 * [Description]: Starts the timer
 * [Args]:
 * 		counterTop: TOP value to be compared with the counter value.
 * 		n_ticksRequired: Number of ticks required to count the required time, given the specified counterTop.
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_start(uint16 counterTop, uint8 n_ticksRequired)
{
	/*set the number of ticks required before notifying the callback. this value will be used by the ISR*/
	g_n_ticksRequired = n_ticksRequired;

	/*enable module interrupt*/
	SET_BIT(TIMSK, OCIE1A);

	/*counter starts from 0*/
	TCNT1 = 0;

	/*counts to counterTop*/
	OCR1A = counterTop;
}

/*-------------------------------------------------------------
 * [Function Name]: Ocu_stop
 * [Description]: Stops the timer
 * [Args]: None
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_stop(void)
{
	/*Disable module interrupt*/
	CLEAR_BIT(TIMSK, OCIE1A);
}


/*-------------------------------------------------------------<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Don't know if i should keep it. Ocu_stop seems like it is doing the job
 * [Function Name]: Ocu_DeInit
 * [Description]: De-initializes the output compare unit
 * [Args]: None
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_deInit(void)
{
	CLEAR_BIT(TCCR1A, FOC1A);
	//CLEAR_BIT(TCCR1A, FOC1B);
	TCCR1A = (TCCR1A & 0x3F) | (0);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B = (TCCR1B & 0xF8) | (0);		/*no clock source*/
}



/*~~~~~~~~~~~~~~~~~~~ ISR ~~~~~~~~~~~~~~~~~~~~~*/
ISR(TIMER1_COMPA_vect)
{
	g_ticksCounter++;
	PORTD = g_ticksCounter;
	//LCD_displayInt(g_n_ticksRequired);
	if(g_ticksCounter == g_n_ticksRequired)
	{
		if(g_Ocu_cbkPtr != NULL_PTR)
		{
			g_Ocu_cbkPtr();
			g_ticksCounter = 0;
		}

	}
}

#if 0
ISR(TIMER1_COMPB_vect)
{
	g_n_ticksRequired++;
		PORTD = g_n_ticksRequired;
		//LCD_displayInt(g_n_ticksRequired);
		if(g_n_ticksRequired == 10)
		{
			if(g_Ocu_cbkPtr != NULL_PTR)
			{
				g_Ocu_cbkPtr();
				g_n_ticksRequired = 0;
			}

		}

}
#endif
