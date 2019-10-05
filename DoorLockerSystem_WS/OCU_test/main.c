/*
 * main.c
 *
 *  Created on: 5 Oct 2019
 *      Author: Home
 */


#include "Ocu.h"

const Ocu_ConfigType s_OcuConfig = {OCU_DISABLE,OCU_PRESCALER_1024};
uint8 flag = 0;

void mainCb()
{
	Ocu_stop();
	PORTC = 0x00;
	flag = 1;
}


int main()
{

	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	SET_BIT(SREG, 7);

	Ocu_setCb(mainCb);

	Ocu_Init(&s_OcuConfig);
	//Ocu_start(46874);
	//PORTC = 0xFF;

	while(1)
	{
		if(flag == 0)
		{
			Ocu_start(46874);
			PORTC = 0xFF;
			flag = 1;
		}
		PORTA = TCNT1L;
		PORTB = TCNT1H;
		//PORTD = TIFR;
	}
}