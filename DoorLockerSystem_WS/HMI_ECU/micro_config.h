/*
 * micro_config.h
 *
 *  Created on: 7 Sep 2019
 *      Author: Home
 */

#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_

#ifndef F_CPU
#define F_CPU 8000000
#endif



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* TESTED BUT NOT USED */
/*DIO*/
#define DIR_A (*(volatile uint8 * const) 0x3A)
#define OUT_A (*(volatile uint8 * const) 0x3B)
#define IN_A (*(const volatile uint8 * const) 0x39)
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7

#define DIR_D (*(volatile uint8 * const) 0x31)
#define OUT_D (*(volatile uint8 * const) 0x32)
#define IN_D (*(const volatile uint8 *const) 0x30)
#define PD0 0
#define PD1 1
#define PD2 2
#define PD3 3
#define PD4 4
#define PD5 5
#define PD6 6
#define PD7 7

#define DIR_C (*(volatile uint8 *) 0x34)
#define OUT_C (*(volatile uint8 *) 0x35)
#define IN_C (*(volatile uint8 *) 0x33)
#define PC0 0
#define PC1 1
#define PC2 2
#define PC3 3
#define PC4 4
#define PC5 5
#define PC6 6
#define PC7 7




#endif /* MICRO_CONFIG_H_ */
