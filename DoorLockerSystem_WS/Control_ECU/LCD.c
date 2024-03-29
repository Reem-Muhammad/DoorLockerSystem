/*
 *
 *	Name		: LCD.c
 *	Author		: Reem Muhammad
 *	Description	: Source file for LCD driver
 *  Created on	: 7 Sep 2019
 *
 */

#include "LCD.h"


/*----------------------------------------
 * [Function Name]: LCD_sendCommand
 * [Description]: sends a command to the LCD
 * [Args]:
 * 		command: command to be sent
 * [Return]: None
 -----------------------------------------*/
void LCD_sendCommand(uint8 command)
{
	/*clear RS to transfer instruction data*/
	CLEAR_BIT(CTRL_PORT_OUT, RS);

	/*Select write mode*/
	CLEAR_BIT(CTRL_PORT_OUT, R_W);

	/*wait at least 50ns (tas)*/
	_delay_ms(1);

	/*enable the LCD*/
	SET_BIT(CTRL_PORT_OUT, EN);

	/*wait at least (tpw - tdsw)ns before sending the command*/
	_delay_ms(1);

#if N_BIT_INTERFACE == 8
	/*send the command*/
	DATA_PORT_OUT = command;

#elif N_BIT_INTERFACE == 4
	/*send the high-nibble of the command first*/
	#if NIBBLE == HIGH_NIBBLE
		DATA_PORT_OUT = (DATA_PORT_OUT & 0x0F) | (command & 0xF0);
	#elif NIBBLE == LOW_NIBBLE
		DATA_PORT_OUT = (DATA_PORT_OUT & 0xF0) | ( (command & 0xF0) >> 4 );
	#endif

		_delay_ms(1);
		/*disable the LCD*/
		CLEAR_BIT(CTRL_PORT_OUT, EN);
		_delay_ms(1);
		/*enable the LCD*/
		SET_BIT(CTRL_PORT_OUT, EN);
		_delay_ms(1);

	/*send the low-nibble of the command*/
	#if NIBBLE == HIGH_NIBBLE
		DATA_PORT_OUT = (DATA_PORT_OUT & 0x0F) | ( (command & 0x0F) << 4 );
	#elif NIBBLE == LOW_NIBBLE
		DATA_PORT_OUT = (DATA_PORT_OUT & 0xF0) | (command & 0x0F);
	#endif
#endif

	/*wait at least (tdsw)ns before clearing the enable line*/
	_delay_ms(1);

	/*disable the LCD*/
	CLEAR_BIT(CTRL_PORT_OUT, EN);

	/*Wait at least (th)ns while holding the data on the line after disabling the LCD*/
	_delay_ms(1);

}



/*----------------------------------------
 * [Function Name]: LCD_displayCharacter
 * [Description]: sends a character to be displayed on the LCD
 * [Args]:
 * 		character: character to be displayed
 * [Return]: None
 -----------------------------------------*/
void LCD_displayCharacter(uint8 character)
{
	/*Set RS to transfer display data*/
	SET_BIT(CTRL_PORT_OUT, RS);

	/*Select write mode*/
	CLEAR_BIT(CTRL_PORT_OUT, R_W);

	/*wait at least 50ns (address setup time)*/
	_delay_ms(1);

	/*enable the LCD*/
	SET_BIT(CTRL_PORT_OUT, EN);

	/*wait at least (tpw - tdsw)ns before sending the command*/
	_delay_ms(1);

#if N_BIT_INTERFACE == 8
	/*send the command*/
	DATA_PORT_OUT = character;

#elif N_BIT_INTERFACE == 4
	/*send the high-nibble of the command first*/
	#if NIBBLE == HIGH_NIBBLE
		DATA_PORT_OUT = (DATA_PORT_OUT & 0x0F) | (character & 0xF0);
	#elif NIBBLE == LOW_NIBBLE
		DATA_PORT_OUT = (DATA_PORT_OUT & 0xF0) | ( (character & 0xF0) >> 4 );
	#endif

		_delay_ms(1);
		/*disable the LCD*/
		CLEAR_BIT(CTRL_PORT_OUT, EN);
		_delay_ms(1);
		/*enable the LCD*/
		SET_BIT(CTRL_PORT_OUT, EN);
		_delay_ms(1);

	/*send the low-nibble of the command*/
	#if NIBBLE == HIGH_NIBBLE
		DATA_PORT_OUT = (DATA_PORT_OUT & 0x0F) | ( (character & 0x0F) << 4 );
	#elif NIBBLE == LOW_NIBBLE
		DATA_PORT_OUT = (DATA_PORT_OUT & 0xF0) | (character & 0x0F);
	#endif
#endif

	/*wait at least (tdsw)ns before clearing the enable line*/
	_delay_ms(1);

	/*disable the LCD*/
	CLEAR_BIT(CTRL_PORT_OUT, EN);

	/*Wait at least (th)ns while holding the data on the line after disabling the LCD*/
	_delay_ms(1);
}



/*----------------------------------------
 * [Function Name]: LCD_displayString
 * [Description]: sends a string to be displayed on the LCD
 * [Args]:
 * 		character_ptr: pointer to the first character of the string to be displayed
 * [Return]: None
 -----------------------------------------*/
void LCD_displayString(uint8 *character_ptr)
{
	/*display the characters until the null terminator is encountered*/
	while(*character_ptr != '\0')
	{
		LCD_displayCharacter(*character_ptr);
		character_ptr++;
	}
}



/*----------------------------------------
 * [Function Name]: LCD_displayInt
 * [Description]: displays an integer number after it has been converted into a string
 * [Args]:
 * 		int_num: a signed 32-bit integer number to be displayed
 * [Return]: None
 -----------------------------------------*/
void LCD_displayInt(sint32 int_num)
{
	uint8 str_of_int[LCD_N_COL]= {0};
	if (int_num < 0)
	{
		LCD_displayCharacter('-');

		/*convert into a positive number*/
		int_num = -1 * int_num;

	}
	/*convert the positive number (int_num) into a string (str_of_int)*/
	convertIntToString(int_num, str_of_int);

	/*display the number*/
	LCD_displayString(str_of_int);
}



/*----------------------------------------
 * [Function Name]: convertIntToString
 * [Description]: takes a positive integer number and converts it into a string
 * [Args]:
 * 		int_num: a signed 32-bit integer number to be converted
 * 		*str_of_int_ptr: pointer to the string that represents the converted integer number
 * [Return]: None
 -----------------------------------------*/
void convertIntToString(uint32 int_num, uint8 *str_of_int_ptr)
{
	/*temporary array that stores the string of int_num in reverse.
	 * the first item is NULL so that it is the last item stored in str_of_int to fit
	 *  LCD_displayString().
	 */
	uint8 temp[LCD_N_COL]={'\0'};

	/*a reference array that stores the ASCII corresponding to each decimal digit*/
	uint8 ASCII_reference[10] = {'0','1','2', '3', '4', '5', '6', '7', '8', '9'};

	/*index starts from 1, since element 0 is the null terminator*/
	uint8 index = 1;

	if(int_num == 0)
	{
		temp[index] = ASCII_reference[0];
		index++;
	}
	else
	{
		while(int_num != 0)
		{
			temp[index] = ASCII_reference[int_num % 10];
			int_num /= 10;
			index++;
		}

	}

	/*store the converted number in str_of_int in the right order*/
	for (uint8 i = 0; i < index; i++)
	{
		str_of_int_ptr[i] = temp[index - i -1];
	}
}



/*----------------------------------------
 * [Function Name]: LCD_init
 * [Description]: initializes the LCD
 * [Args]: None
 * [Return]: None
 -----------------------------------------*/
void LCD_init()
{
	/*Configure the control pins as o/p*/
	CTRL_PORT_DIR |= (1<<RS) | (1<<R_W) | (1<<EN);

#if N_BIT_INTERFACE == 8
	/*Configure data pins as i/p*/
	DATA_PORT_DIR = 0xFF;

	/*switch to 2 lines, 8-bit mode*/
	LCD_sendCommand(TWO_LINE_8BIT_MODE);


#elif N_BIT_INTERFACE == 4
	/*sets either the high-nibble or the low-nibble (as configured) to o/p*/
	DATA_PORT_DIR |= NIBBLE;

	/*switch to 2 lines, 4-bit mode*/
	LCD_sendCommand(0x02);
	LCD_sendCommand(TWO_LINE_4BIT_MODE);

#endif

	LCD_sendCommand(CLEAR_DISPLAY_CLEAR_DDRAM);
	LCD_sendCommand(DISPLAY_ON_CURSOR_OFF);
	LCD_sendCommand(CLEAR_DISPLAY);
}



/*----------------------------------------
 * [Function Name]: LCD_clear
 * [Description]: clears the display
 * [Args]: None
 * [Return]: None
 -----------------------------------------*/
void LCD_clear()
{
	LCD_sendCommand(CLEAR_DISPLAY);
}



/*----------------------------------------
 * [Function Name]: LCD_goToRowCol
 * [Description]: sets the cursor position on the display
 * [Args]:
 * 		row: the desired row number, starting from 0
 * 		col: the desired column number, starting from 0
 * [Return]: None
 -----------------------------------------*/
void LCD_goToRowCol(uint8 row, uint8 col)
{
	/*To set the cursor position on the display, use the command: (0x80 + address)*/
	switch(row)
	{
	case 0:
		LCD_sendCommand( 0x80 + (FIRST_LINE_BASE_ADDRESS + col) );
		break;
	case 1:
		LCD_sendCommand( 0x80 + (SECOND_LINE_BASE_ADDRESS + col) );
		break;
	default:
		break;
	}
}
