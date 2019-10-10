 /*
 *
 *	Name		: Ocu.h
 *	Author		: Reem Muhammad
 *	Description	: Header file for OCU driver
 *  Created on: 4 Oct 2019
 *
 */

#ifndef OCU_H_
#define OCU_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

//#define CHANNEL_1A 0
//#define CHANNEL_1B 1

/*~~~~~~~~~~~~ OCU Resolution ~~~~~~~~~~~~~~~~~~*/
#define OCU_8MHZ_1MS_TOP 7999
#define OCU_4MHZ_1MS_TOP 3999
#define OCU_2MHZ_1MS_TOP 1999
#define OCU_1MHZ_1MS_TOP 999

#define OCU_8MHZ_1MS_PRESCALER 0b001  /*Prescaler value: 1*/
#define OCU_4MHZ_1MS_PRESCALER 0b001
#define OCU_2MHZ_1MS_PRESCALER 0b001
#define OCU_1MHZ_1MS_PRESCALER 0b001

#define OCU_8MHZ_1S_TOP 31249
#define OCU_4MHZ_1S_TOP 62499
#define OCU_2MHZ_1S_TOP 31249
#define OCU_1MHZ_1S_TOP 15624

#define OCU_8MHZ_1S_PRESCALER 0b100  /*Prescaler value: 256*/
#define OCU_4MHZ_1S_PRESCALER 0b011	 /*Prescaler value: 64*/
#define OCU_2MHZ_1S_PRESCALER 0b011  /*Prescaler value: 64*/
#define OCU_1MHZ_1S_PRESCALER 0b011  /*Prescaler value: 64*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~~~~~~~ Type Definitions ~~~~~~~~~~~~~~~~~~~~~~*/
/*Numeric identifier of an OCU channel*/
typedef uint8 Ocu_ChannelType;		//not used yet

/*for reading the counter and writing threshold values (in number of ticks)*/
typedef uint16 Ocu_ValueType;

/*Output state of the pin attached to an OCU channel*/
typedef enum
{
	OCU_LOW, OCU_HIGH
}Ocu_PinStateType;

/*Automatic action to be performed on the pin attached to an OCU channel.*/
typedef enum
{
	OCU_PIN_DISABLE, OCU_PIN_TOGGLE, OCU_PIN_SET_LOW, OCU_PIN_SET_HIGH
}Ocu_PinActionType;

/*To set the resolution of the timer*/
typedef enum
{
	OCU_RESOLUTION_1MS, OCU_RESOLUTION_1S
}Ocu_TickResolutionType;

/*Initialization data for the OCU driver*/
typedef struct
{
	Ocu_TickResolutionType e_OcuResolution;
}Ocu_ConfigType;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




/*~~~~~~~~~~~~~~~~~~~~~~~ Functions Definitions ~~~~~~~~~~~~~~~~~~~~~*/
void Ocu_init(const Ocu_ConfigType* ConfigPtr);
void Ocu_deInit(void);
void Ocu_SetPinAction(Ocu_PinActionType PinAction);
void Ocu_setCbk( void (*cbkPtr)(void) );
void Ocu_start(Ocu_ValueType Threshold);
void Ocu_stop(void);



//void Ocu_SetPinAction(Ocu_ChannelType ChannelNumber, Ocu_PinActionType PinAction);
//void Ocu_SetAbsoluteThreshold( Ocu_ChannelType ChannelNumber, Ocu_ValueType AbsoluteValue);
//void Ocu_Notification_0(void);
//void Ocu_Notification_1(void);



#endif /* OCU_H_ */
