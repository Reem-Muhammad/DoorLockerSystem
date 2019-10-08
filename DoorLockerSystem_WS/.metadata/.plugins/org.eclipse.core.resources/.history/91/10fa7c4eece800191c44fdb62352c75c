/*
 * I2C.h
 *
 *  Created on: 26 Sep 2019
 *      Author: Home
 */

#ifndef I2C_H_
#define I2C_H_

#include "micro_config.h"
#include "common_macros.h"
#include "std_types.h"

#warning "STATUS CODES ARE NOT COMPLETE YET"


#define TWI_STATUS (TWSR & 0xFB)
#define READ 0b1
#define WRITE 0b0

/*
 * ~~~~~~~~ Status Codes ~~~~~~~~~~~
 */
/*Master Transmit Mode*/
#define START_CONDITION_TRANSMITTED 0x08
#define REPEATED_START_CONDITION_TRANSMITTED 0x10
#define MT_SLA_W_TRANSMITTED_ACK_RECEIVED 0x18
#define MT_SLA_W_TRANSMITTED_NACK_RECEIVED 0x20
#define MT_DATA_TRANSMITTED_ACK_RECEIVED 0x28
#define MT_DATA_TRANSMITTED_NACK_RECEIVED 0x30
#define MT_ARBITRATION_LOST 0x38

/*Master Receive Mode*/
#define MR_SLA_R_TRANSMITTED_ACK_RECEIVED 0x40
#define MR_SLA_R_TRANSMITTED_NACK_RECEIVED 0x48
#define MR_DATA_RECEIVED_ACK_RETURNED 0x50
#define MR_DATA_RECEIVED_NACK_RETURNED 0x58

/*Slave Transmit Mode: NOT COMPLETE*/
#define ST_SLA_R_RECEIVED_ACK_RETURNED 0xA8
#define ST_DATA_TRANSMITTED_ACK_RECEIVED 0xB8
#define ST_DATA_TRANSMITTED_NACK_RECEIVED 0xC0

/*Slave Receive Mode: NOT COMPLETE*/
#define SLA_W_RECEIVED_ACK_RETURNED 0x60
#define GENERAL_CALL_RECEIVED_ACK_RETURNED 0x70


typedef enum
{
	TWI_PRESCALER_VALUE_1, TWI_PRESCALER_VALUE_4, TWI_PRESCALER_VALUE_16, TWI_PRESCALER_VALUE_64
}Twi_PrescalerType;

typedef enum
{
	GENERAL_CALL_RECOGNITION_DISABLED, GENERAL_CALL_RECOGNITION_ENABLED
}Twi_GeneralCallRecognition;

typedef struct
{
	Twi_PrescalerType e_twi_prescaler; /*TWPS*/
	Twi_GeneralCallRecognition e_twi_general_call_recognition;

	uint8 twi_address;
	uint8 twi_bit_rate; /*TWBR*/

}Twi_ConfigType;


/*
 * ~~~~~~~~ Functions Definitions ~~~~~~~~~~~
 */
void TWI_init(const Twi_ConfigType* config_ptr);
void TWI_start();
void TWI_write(const uint8 byte_to_write);
uint8 TWI_readWithACK();
uint8 TWI_readWithNACK();
void TWI_stop();
//uint8 TWI_getStatus();
#endif /* I2C_H_ */
