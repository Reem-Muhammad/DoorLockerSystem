/*
 * Ocu.c
 *
 *  Created on: 4 Oct 2019
 *      Author: Home
 */

#include "Ocu.h"
#include "LCD.h"

/*Global pointer to the configuration set, so it is accessible by other functions*/
volatile const Ocu_ConfigType *g_Ocu_ConfigPtr;
volatile uint8 g_n_interrupts = 0;
void (*g_Ocu_NotificationPtr)() = NULL_PTR;

void Ocu_setCb( void (*cbNotificationPtr)(void))
{
	g_Ocu_NotificationPtr = cbNotificationPtr;
}

/*-------------------------------------------------------------
 * [Function Name]: Ocu_Init
 * [Description]: initializes the output compare unit
 * [Args]:
 * 		ConfigPtr: pointer to the configuration set
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_Init(const Ocu_ConfigType *ConfigPtr)
{
	g_Ocu_ConfigPtr = ConfigPtr;

	/*Set FOC1 for non-PWM mode */
		SET_BIT(TCCR1A, FOC1A);
		//SET_BIT(TCCR1A, FOC1B);

	TCCR1A = (TCCR1A & 0x3F) | (ConfigPtr->e_ocu_pinAction << 6);	/*set OC1A behavior*/
	TCCR1A = (TCCR1A & 0xCF) | (ConfigPtr->e_ocu_pinAction << 4);	/*set OC1B behavior*/


	/*the counter starts from 0*/
	//TCNT1 = 0; //----->

	/*set the source for max counter value to be OCR1A register: WGM13:0 -> 0 1 0 0 */
	//TCCR1B |= (1<<WGM12);

	//OCR1A = 46874; //----->

	/*set the prescaler*/
	//TCCR1B = (TCCR1B & 0xF8) | (ConfigPtr->e_ocu_prescaler);
	TCCR1B =(1<<CS12) | (1<<CS10) | (1<<WGM12);

	/*Enable module interrupt*/
	//SET_BIT(TIMSK, OCIE1A); //----->
	//SET_BIT(TIMSK, OCIE1B);

	//PORTA = TCCR1A;

}

void Ocu_start(uint16 top)
{
	SET_BIT(TIMSK, OCIE1A);		/*enable the module interrupt*/
	TCNT1 = 0;		/*the counter starts from 0*/
	OCR1A = top;	/*counts to top*/

}

void Ocu_stop(void)
{
	CLEAR_BIT(TIMSK, OCIE1A);
}
/*-------------------------------------------------------------
 * [Function Name]: Ocu_DeInit
 * [Description]: De-initializes the output compare unit
 * [Args]: None
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_DeInit(void)
{
	TCCR1B = (TCCR1B & 0xF8) | (0);		/*no clock source*/
}

/*-------------------------------------------------------------
 * [Function Name]: Ocu_SetPinAction
 * [Description]: sets the action to be performed by hardware automatically, at the next compare match in the corresponding OCU channel
 * [Args]:
 * 		ChannelNumber: Numeric identifier of the OCU channel (CHANNEL_1A or CHANNEL_1B)
 * 		PinAction: behavior on compare match
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_SetPinAction(Ocu_ChannelType ChannelNumber, Ocu_PinActionType PinAction)
{
	TCCR1A = (TCCR1A & 0x3F) | (PinAction << 6);	/*set OC1A behavior*/
	TCCR1A = (TCCR1A & 0xCF) | (PinAction << 4);	/*set OC1B behavior*/
#if 0
	switch(ChannelNumber)
	{
	case CHANNEL_1A:
		TCCR1A = (TCCR1A & 0x3F) | (PinAction << 6);
		break;
	case CHANNEL_1B:
		TCCR1A = (TCCR1A & 0xCF) | (PinAction << 4);
		break;
	default:
		break;
	}
#endif
}

/*-------------------------------------------------------------
 * [Function Name]: Ocu_SetAbsoluteThreshold
 * [Description]: sets the compare value for the specified channel
 * [Args]:
 * 		ChannelNumber: Numeric identifier of the OCU channel (CHANNEL_1A or CHANNEL_1B)
 * 		AbsoluteValue: Value to compare with the content of the counter
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_SetAbsoluteThreshold( Ocu_ChannelType ChannelNumber, Ocu_ValueType AbsoluteValue)
{
	/*set the compare value (number of ticks required to trigger the interrupt)*/
	OCR1A = AbsoluteValue;
}

/*-------------------------------------------------------------
 * [Function Name]: Ocu_Notification_0
 * [Description]: notification function that is called when a compare match occurs on channel 1A
 * [Args]: None
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_Notification_0(void)
{

}


/*-------------------------------------------------------------
 * [Function Name]: Ocu_Notification_1
 * [Description]: notification function that is called when a compare match occurs on channel 1B
 * [Args]: None
 * [Return]: None
 --------------------------------------------------------------*/
void Ocu_Notification_1(void)
{

}



/*~~~~~~~~~~~~~~~~~~~ ISR ~~~~~~~~~~~~~~~~~~~~~*/
ISR(TIMER1_COMPA_vect)
{
	g_n_interrupts++;
	PORTD = g_n_interrupts;
	//LCD_displayInt(g_n_interrupts);
	if(g_n_interrupts == 10)
	{
		if(g_Ocu_NotificationPtr != NULL_PTR)
		{
			g_Ocu_NotificationPtr();
			g_n_interrupts = 0;
		}

	}
}

#if 0
ISR(TIMER1_COMPB_vect)
{
	g_n_interrupts++;
		PORTD = g_n_interrupts;
		//LCD_displayInt(g_n_interrupts);
		if(g_n_interrupts == 10)
		{
			if(g_Ocu_NotificationPtr != NULL_PTR)
			{
				g_Ocu_NotificationPtr();
				g_n_interrupts = 0;
			}

		}

}
#endif
