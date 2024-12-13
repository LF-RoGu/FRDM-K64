/*
 * UART_TERM.c
 *
 *  Created on: Nov 5, 2018
 *      Author: LuisFernando
 */

#include "UART_TERM.h"

static uint8 data[SIZE];
static uint8 position = FALSE;

void TERA_menu()
{
	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(UART_0,"\033[2J");
	/** VT100 command for text in red and background in green*/
	UART_putString(UART_0, "\033[0;37;41m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[10;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "1) Establecer Hora\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[11;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "2) Establecer Fecha\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[12;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "3) Leer hora\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[13;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "4) Leer fecha\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[14;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "5) Escribir mensaje en memoria I2C\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[15;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "6) Mostrar mensaje\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[17;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "Opcion\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[18;10H");
}
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/
void show_set_hour()
{
	/*Funct to print in the screen the menu to set the hour*/
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(UART_0, "\033[2J");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[10;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "Write in format HH/MM\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[11;10H");
}

void get_hour()
{
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(UART_0,"\033[2J");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[10;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "RealTime:\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[11;10H");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_hour());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0,get_decena());
	UART_putChar(UART_0,get_unidad());
	/*Prints : for the format*/
	UART_putString(UART_0, ":");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_min());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_decena());
	UART_putChar(UART_0, get_unidad());
	/*Prints : for the format*/
	UART_putString(UART_0, ":");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_sec());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_decena());
	UART_putChar(UART_0, get_unidad());
}

void refresh_hour()
{
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[11;10H");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_hour());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_decena());
	UART_putChar(UART_0, get_unidad());
	/*Prints : for the format*/
	UART_putString(UART_0, ":");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_min());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_decena());
	UART_putChar(UART_0, get_unidad());
	/*Prints : for the format*/
	UART_putString(UART_0, ":");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_sec());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_decena());
	UART_putChar(UART_0, get_unidad());
}

void change_hour()
{
	if((ASCII_NUMBER_0 <= get_key()) && (ASCII_NUMBER_9 >= get_key()))
	{
		/*Store the hour that the user type in*/
		data[position] = get_key();
		/*Move in the buffer*/
		position++;
		if(SIZE < position)
		{
			RTC_set_hour((data[BIT0]*TEN)+data[BIT1]);
			RTC_set_min((data[BIT2]*TEN)+data[BIT3]);
			/*Reset the index*/
			position = FALSE;
			/*Set return state, so we can return to the menu*/
			fptr_function(RETURN);
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[20;10H");
			/** Set the text in a string*/
			UART_putString(UART_0, "Time has been changed\r");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[21;10H");
			/** Set the text in a string*/
			UART_putString(UART_0, "Press any key to continue...\r");
		}
	}
}
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/
void show_set_date(void)
{
	/*Funct to print in the screen the menu to set the date*/
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(UART_0, "\033[2J");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[10;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "Write in format DD/MM/YY\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[11;10H");
}

void get_date(void)
{
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(UART_0, "\033[2J");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[10;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "Date:\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[11;10H");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_date());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_unidad());
	UART_putChar(UART_0, get_decena());
	/*Prints : for the format*/
	UART_putString(UART_0, "/");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_month());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_unidad());
	UART_putChar(UART_0, get_decena());
	/*Prints : for the format*/
	UART_putString(UART_0, "/");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_year());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_unidad());
	UART_putChar(UART_0, get_decena());
}

void refresh_date()
{
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[11;10H");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_date());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_unidad());
	UART_putChar(UART_0, get_decena());
	/*Prints : for the format*/
	UART_putString(UART_0, "/");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_month());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_unidad());
	UART_putChar(UART_0, get_decena());
	/*Prints : for the format*/
	UART_putString(UART_0, "/");
	/** Gets the value in the register of time, corresponding to Hrs*/
	value_capture(RTC_get_year());
	/** Print the value of the register once it got deco*/
	UART_putChar(UART_0, get_unidad());
	UART_putChar(UART_0, get_decena());
}
void change_date()
{
	if((ASCII_NUMBER_0 <= get_key()) && (ASCII_NUMBER_9 >= get_key()))
	{
		/*Store the hour that the user type in*/
		data[position] = get_key();
		/*Move in the buffer*/
		position++;
		if(SIZE < position)
		{
			RTC_set_year((data[BIT0]*TEN)+data[BIT1]);
			RTC_set_month((data[BIT2]*TEN)+data[BIT3]);
			RTC_set_date((data[BIT4]*TEN)+data[BIT5]);
			/*Reset the index*/
			position = FALSE;
			/*Set return state, so we can return to the menu*/
			fptr_function(RETURN);
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[20;10H");
			/** Set the text in a string*/
			UART_putString(UART_0, "Date has been changed\r");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[21;10H");
			/** Set the text in a string*/
			UART_putString(UART_0, "Press any key to continue...\r");
		}
	}
}
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/
void num_mem()
{
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(UART_0, "\033[2J");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[10;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "Num of message from 1 to 5\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[11;10H");
}
void write_mem()
{
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(UART_0, "\033[2J");
	/** VT100 command for text in red and background in green*/
	UART_putString(UART_0, "\033[0;37;40m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[12;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "Write text to storage\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[13;10H");
}
void show_mem()
{
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(UART_0, "\033[2J");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;31;40m");
	/** VT100 command for text in black and background in black*/
	UART_putString(UART_0,"\033[0;33;40m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[12;10H");
	/** Set the text in a string*/
	UART_putString(UART_0, "Text stored in memory:\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0, "\033[13;10H");
}
