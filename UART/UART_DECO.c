/*
 * UART_DECO.c
 *
 *  Created on: Nov 5, 2018
 *      Author: LuisFernando
 */

#include "UART_DECO.h"

static uint8 temp0 = FALSE,	/*Unidad*/
			 temp1 = FALSE,	/*Decena*/
			 temp2 = FALSE,	/*Centena*/
			 temp3 = FALSE,	/*Millar*/
			 temp4 = FALSE; /*Decena Millar*/

static uint8 key_flag = FALSE,
			 enter_flag = FALSE,
			 esc_flag = FALSE,
			 state_flag = FALSE;

static uint8 buffer[SIZE];
static uint8 buffer_text[TXT_SIZE];
static uint8 position;

/**This is mail box to received the information from the serial port*/
extern UART_MailBoxType UART0_MailBox;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*SECTION TO DECO THE VALUE OBTAINED FROM THE MEMORY*/

void value_capture(uint32 value)
{
	uint8 tempValue4 = value / DECENA_MILLAR;
	uint8 tempValue3 = value / MILLAR;
	uint8 tempValue2 = value / CENTENA;
	uint8 tempValue1 = value / DECENA;

	/*Decena del Millar*/
	if (FALSE != tempValue4)
		temp4 = tempValue4;
	else
		temp4 = 0;
	/*Millar*/
	if (FALSE != tempValue3) {
		value = value - (DECENA_MILLAR * temp4);
		temp3 = value / MILLAR;
	} else
		temp3 = 0;
	/*Centena*/
	if (FALSE != tempValue2) {
		value = value - (MILLAR * temp3);
		temp2 = value / CENTENA;
	} else
		temp2 = 0;
	/*Decena*/
	if (FALSE != tempValue1) {
		value = value - (CENTENA * temp2);
		temp1 = value / DECENA;
	} else
		temp1 = 0;
	/*Unidad*/
	if (FALSE != value)
		temp0 = value - (DECENA * temp1);
	else
		temp0 = 0;
}

uint8 get_unidad()
{
	return (temp0 + ASCII_CONV);
}
uint8 get_decena()
{
	return (temp1 + ASCII_CONV);
}
uint8 get_centena()
{
	return (temp2 + ASCII_CONV);
}
uint8 get_millar()
{
	return (temp3 + ASCII_CONV);
}
uint8 get_decenaMillar()
{
	return (temp4 + ASCII_CONV);
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*SECTION*/
void set_state()
{
	uint8 data_help;
	uint8 n_message;
	uint16 addres_aux = ADDRESS_INIT;
	uint8 buffer_cont = FALSE;
	uint8 txt_cont = BIT1;
	uint8 enter_cont = FALSE;
	if (UART0_MailBox.flag)
	{
		if (ASCII_NUMBER_1 == UART0_MailBox.mailBox) {
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[16;20H");
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, get_key());
			set_key(ASCII_NUMBER_1);
		}

		if (ASCII_NUMBER_2 == UART0_MailBox.mailBox) {/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[16;20H");
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, get_key());
			set_key(ASCII_NUMBER_2);
		}

		if (ASCII_NUMBER_3 == UART0_MailBox.mailBox) {
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[16;20H");
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, get_key());
			set_key(ASCII_NUMBER_3);
		}

		if (ASCII_NUMBER_4 == UART0_MailBox.mailBox) {
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[16;20H");
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, get_key());
			set_key(ASCII_NUMBER_4);
		}

		if (ASCII_NUMBER_5 == UART0_MailBox.mailBox) {
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[16;20H");
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, get_key());
			set_key(ASCII_NUMBER_5);
		}

		if (ASCII_NUMBER_6 == UART0_MailBox.mailBox) {
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0, "\033[16;20H");
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, get_key());
			set_key(ASCII_NUMBER_6);
		}

		if (ASCII_ESC == UART0_MailBox.mailBox) {
			set_esc(ASCII_ESC);
		}

		if (ASCII_ENTER == UART0_MailBox.mailBox) {
			set_enter(ASCII_ENTER);
		}
			/**********************************************/
			if ((TRUE == get_enter()) && (ASCII_NUMBER_1 == get_key())) {
				set_key(ASCII_NUMBER_0);
				show_set_hour();
				enter_flag = FALSE;
				state_flag = TRUE;
				UART0_MailBox.flag = FALSE;
				while(state_flag)
				{
					if (UART0_MailBox.flag)
					{
						buffer[position] = UART0_MailBox.mailBox;
						buffer[position] = buffer[position] - ASCII_CONV;
						/**Sends to the PCA the received data in the mailbox*/
						UART_putChar(UART_0, UART0_MailBox.mailBox);
						position++;
						if((position == 2) || (position == 4))
						{
							/**Sends to the PCA the received data in the mailbox*/
							UART_putChar(UART_0, '/');
						}
						if(position > SIZE - 1)
						{
							data_help = ((buffer[BIT0] * TEN) + buffer[BIT1]);
							RTC_set_hour(data_help);
							data_help = ((buffer[BIT2] * TEN) + buffer[BIT3]);
							RTC_set_min(data_help);
							/** VT100 command for positioning the cursor in x and y position*/
							UART_putString(UART_0, "\033[12;10H");
							/** Set the text in a string*/
							UART_putString(UART_0, "Time has change...\r");
							/*Reset index*/
							position = FALSE;
							/*Exit loop*/
							state_flag = FALSE;
							delay(DELAY);
						}
					}
					UART0_MailBox.flag = FALSE;
				}
			}
			if ((TRUE == get_enter()) && (ASCII_NUMBER_2 == get_key())) {
				set_key(ASCII_NUMBER_0);
				show_set_date();
				enter_flag = FALSE;
				state_flag = TRUE;
				UART0_MailBox.flag = FALSE;
				while(state_flag)
				{
					if (UART0_MailBox.flag)
					{
						buffer[position] = UART0_MailBox.mailBox;
						buffer[position] = buffer[position] - ASCII_CONV;
						/**Sends to the PCA the received data in the mailbox*/
						UART_putChar(UART_0, UART0_MailBox.mailBox);
						position++;
						if (FALSE == (position % 2))
						{
							/**Sends to the PCA the received data in the mailbox*/
							UART_putChar(UART_0, '/');
						}
						while(position > SIZE-1)
						{
							data_help = ((buffer[BIT0] * TEN) + buffer[BIT1]);
							RTC_set_year(data_help);
							data_help = ((buffer[BIT2] * TEN) + buffer[BIT3]);
							RTC_set_month(data_help);
							data_help = ((buffer[BIT4] * TEN) + buffer[BIT5]);
							RTC_set_date(data_help);
							/** VT100 command for positioning the cursor in x and y position*/
							UART_putString(UART_0, "\033[12;10H");
							/** Set the text in a string*/
							UART_putString(UART_0, "Date has change...\r");
							/*Reset index*/
							position = FALSE;
							/*Exit loop*/
							state_flag = FALSE;
							/*Clear keyboard flags*/
							enter_flag = FALSE;
							esc_flag = TRUE;
						}
					}
					UART0_MailBox.flag = FALSE;
				}
			}
			if ((TRUE == get_enter()) && (ASCII_NUMBER_3 == get_key())) {
				set_key(ASCII_NUMBER_0);
				get_hour();
				enter_flag = FALSE;
				state_flag = TRUE;
				UART0_MailBox.flag = FALSE;
				while(state_flag)
				{
					if(TRUE == PIT0_get_interrupt_status())
					{
						refresh_hour();
					}

					if (UART0_MailBox.flag)
					{
						set_key(UART0_MailBox.mailBox);
						if((ASCII_ENTER == get_key()) || (ASCII_ESC == get_key()))
						{
							if(ASCII_ENTER == get_key())
							{
								set_enter(get_key());
							}
							if(ASCII_ESC == get_key())
							{
								set_esc(get_key());
							}
						}
						if((TRUE == get_enter()) || (TRUE == get_esc()))
						{
							state_flag = FALSE;
							enter_flag = FALSE;
							esc_flag = TRUE;
						}
					}
					UART0_MailBox.flag = FALSE;
				}
				UART0_MailBox.flag = FALSE;
			}
			if ((TRUE == get_enter()) && (ASCII_NUMBER_4 == get_key())) {
				set_key(ASCII_NUMBER_0);
				get_date();
				enter_flag = FALSE;
				state_flag = TRUE;
				UART0_MailBox.flag = FALSE;
				while(state_flag)
				{
					if (TRUE == PIT0_get_interrupt_status())
					{
						refresh_date();
					}
					if (UART0_MailBox.flag)
					{
						set_key(UART0_MailBox.mailBox);
						if((ASCII_ENTER == get_key()) || (ASCII_ESC == get_key()))
						{
							if (ASCII_ENTER == get_key())
							{
								set_enter(get_key());
							}
							if (ASCII_ESC == get_key())
							{
								set_esc(get_key());
							}

						}
						if((TRUE == get_enter()) || (TRUE == get_esc()))
						{
							state_flag = FALSE;
							enter_flag = FALSE;
							esc_flag = TRUE;
						}
					}
					UART0_MailBox.flag = FALSE;
				}
				UART0_MailBox.flag = FALSE;
			}
			if ((TRUE == get_enter()) && (ASCII_NUMBER_5 == get_key())) {
				set_key(ASCII_NUMBER_0);
				num_mem();
				enter_flag = FALSE;
				state_flag = TRUE;
				UART0_MailBox.flag = FALSE;
				while(state_flag)
				{
					if (UART0_MailBox.flag)
					{
						n_message = UART0_MailBox.mailBox;
						UART_putChar(UART_0, n_message);
						n_message -= ASCII_CONV;
						switch(n_message)
						{
						case MASSAGE_1:
							write_mem();
							/** VT100 command for positioning the cursor in x and y position*/
							UART_putString(UART_0, "\033[12;10H");
							while(enter_cont < 1)
							{

								if (UART0_MailBox.flag)
								{
									set_key(UART0_MailBox.mailBox);
									UART_putChar(UART_0, UART0_MailBox.mailBox);
									buffer_text[buffer_cont] = UART0_MailBox.mailBox;
									buffer_cont++;
									if(buffer_cont == TXT_SIZE)
									{
										set_key(ASCII_ENTER);
									}
									if((ASCII_ENTER == get_key()))
									{
										enter_cont++;
										state_flag = FALSE;
										enter_flag = FALSE;
										esc_flag = TRUE;
										buffer_cont = BIT1;
										for(addres_aux = FALSE;addres_aux < TXT_SIZE;addres_aux++)
										{
											/*Send data stored in the buffer to the memory via I2C*/
											EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
											/*Clear the buffer*/
											buffer_text[buffer_cont] = FALSE;
											/*Increase buffer*/
											buffer_cont++;
										}
									}
								}
								UART0_MailBox.flag = FALSE;
							}
							break;
						case MASSAGE_2:
							write_mem();
							/** VT100 command for positioning the cursor in x and y position*/
							UART_putString(UART_0, "\033[12;10H");
							while(enter_cont < 2)
							{
								if (UART0_MailBox.flag)
								{
									set_key(UART0_MailBox.mailBox);
									UART_putChar(UART_0, UART0_MailBox.mailBox);
									buffer_text[buffer_cont] = UART0_MailBox.mailBox;
									buffer_cont++;
									if(buffer_cont == BIT2*TXT_SIZE)
									{
										set_key(ASCII_ENTER);
									}
									if((ASCII_ENTER == get_key()))
									{
										enter_cont++;
										buffer_cont = BIT1;
										if (enter_cont == 1)
										{
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[14;10H");
											for (addres_aux = BIT1; addres_aux < TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 2) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[15;10H");
											state_flag = FALSE;
											enter_flag = FALSE;
											esc_flag = TRUE;
											for (addres_aux = TXT_SIZE; addres_aux < BIT2*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
									}
								}
								UART0_MailBox.flag = FALSE;
							}
							break;
						case MASSAGE_3:
							write_mem();
							/** VT100 command for positioning the cursor in x and y position*/
							UART_putString(UART_0, "\033[12;10H");
							while(enter_cont < 3)
							{
								if (UART0_MailBox.flag)
								{
									set_key(UART0_MailBox.mailBox);
									UART_putChar(UART_0, UART0_MailBox.mailBox);
									buffer_text[buffer_cont] = UART0_MailBox.mailBox;
									buffer_cont++;
									if(buffer_cont == TXT_SIZE)
									{
										set_key(ASCII_ENTER);
									}
									if((ASCII_ENTER == get_key()))
									{
										enter_cont++;
										buffer_cont = BIT1;
										if (enter_cont == 1) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[14;10H");
											for (addres_aux = BIT1; addres_aux < TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 2) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[15;10H");
											for (addres_aux = BIT2*TXT_SIZE; addres_aux < BIT3*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 3) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[16;10H");
											state_flag = FALSE;
											enter_flag = FALSE;
											esc_flag = TRUE;
											for (addres_aux = BIT3*TXT_SIZE; addres_aux < BIT4*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
									}
								}
								UART0_MailBox.flag = FALSE;
							}
							break;
						case MASSAGE_4:
							write_mem();
							/** VT100 command for positioning the cursor in x and y position*/
							UART_putString(UART_0, "\033[12;10H");
							while(enter_cont < 4)
							{
								if (UART0_MailBox.flag)
								{
									set_key(UART0_MailBox.mailBox);
									UART_putChar(UART_0, UART0_MailBox.mailBox);
									buffer_text[buffer_cont] = UART0_MailBox.mailBox;
									buffer_cont++;
									if(buffer_cont == TXT_SIZE)
									{
										set_key(ASCII_ENTER);
									}
									if((ASCII_ENTER == get_key()))
									{
										enter_cont++;
										buffer_cont = BIT1;
										if (enter_cont == 1) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[14;10H");
											for (addres_aux = BIT1; addres_aux < TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 2) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[15;10H");
											for (addres_aux = BIT2*TXT_SIZE; addres_aux < BIT3*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 3) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[16;10H");
											for (addres_aux = BIT3*TXT_SIZE; addres_aux < BIT4*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 4) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[17;10H");
											state_flag = FALSE;
											enter_flag = FALSE;
											esc_flag = TRUE;
											for (addres_aux = BIT4*TXT_SIZE; addres_aux < BIT5*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
									}
								}
								UART0_MailBox.flag = FALSE;
							}
							break;
						case MASSAGE_5:
							write_mem();
							/** VT100 command for positioning the cursor in x and y position*/
							UART_putString(UART_0, "\033[12;10H");
							while(enter_cont < 5)
							{
								if (UART0_MailBox.flag)
								{
									set_key(UART0_MailBox.mailBox);
									UART_putChar(UART_0, UART0_MailBox.mailBox);
									buffer_text[buffer_cont] = UART0_MailBox.mailBox;
									buffer_cont++;
									if(buffer_cont == TXT_SIZE)
									{
										set_key(ASCII_ENTER);
									}
									if((ASCII_ENTER == get_key()))
									{
										enter_cont++;
										buffer_cont = BIT1;
										if (enter_cont == 1) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[14;10H");
											for (addres_aux = BIT1; addres_aux < TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 2) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[15;10H");
											for (addres_aux = BIT2*TXT_SIZE; addres_aux < BIT3*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 3) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[16;10H");
											for (addres_aux = BIT3*TXT_SIZE; addres_aux < BIT4*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 4) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[17;10H");
											for (addres_aux = BIT4*TXT_SIZE; addres_aux < BIT5*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
										if (enter_cont == 5) {
											/** VT100 command for positioning the cursor in x and y position*/
											UART_putString(UART_0, "\033[18;10H");
											state_flag = FALSE;
											enter_flag = FALSE;
											esc_flag = TRUE;
											for (addres_aux = BIT5*TXT_SIZE; addres_aux < BIT6*TXT_SIZE; addres_aux++)
											{
												/*Send data stored in the buffer to the memory via I2C*/
												EEPROM_write_mem(addres_aux, buffer_text[buffer_cont]);
												/*Clear the buffer*/
												buffer_text[buffer_cont] = FALSE;
												/*Increase buffer*/
												buffer_cont++;
											}
										}
									}
								}
								UART0_MailBox.flag = FALSE;
							}
							break;
						}
					}
					UART0_MailBox.flag = FALSE;
				}
			}
			if ((TRUE == get_enter()) && (ASCII_NUMBER_6 == get_key())) {
				set_key(ASCII_NUMBER_0);
				num_mem();
				enter_flag = FALSE;
				state_flag = TRUE;
				UART0_MailBox.flag = FALSE;
				while(state_flag)
				{
					if (UART0_MailBox.flag)
					{
						n_message = UART0_MailBox.mailBox;
						UART_putChar(UART_0, n_message);
						n_message -= ASCII_CONV;
						buffer_cont = BIT0;
						switch(n_message)
						{
						case MASSAGE_1:
							for(addres_aux = BIT1;addres_aux < TXT_SIZE;addres_aux++)
							{
								buffer[buffer_cont] = EEPROM_read_mem(addres_aux);
								buffer_cont++;
							}
							break;
						case MASSAGE_2:
							for (addres_aux = BIT2*TXT_SIZE; addres_aux < BIT3*TXT_SIZE; addres_aux++)
							{
								buffer[buffer_cont] = EEPROM_read_mem(addres_aux);
								buffer_cont++;
							}
							break;
						case MASSAGE_3:
							for (addres_aux = BIT3*TXT_SIZE; addres_aux < BIT4*TXT_SIZE; addres_aux++)
							{
								buffer[buffer_cont] = EEPROM_read_mem(addres_aux);
								buffer_cont++;
							}
							break;
						case MASSAGE_4:
							for (addres_aux = BIT4*TXT_SIZE; addres_aux < BIT5*TXT_SIZE; addres_aux++)
							{
								buffer[buffer_cont] = EEPROM_read_mem(addres_aux);
								buffer_cont++;
							}
							break;
						case MASSAGE_5:
							for (addres_aux = BIT5*TXT_SIZE; addres_aux < BIT6*TXT_SIZE; addres_aux++)
							{
								buffer[buffer_cont] = EEPROM_read_mem(addres_aux);
								buffer_cont++;
							}
							break;
						}
					}
					if((ASCII_ENTER == get_key()))
					{
						state_flag = FALSE;
						enter_flag = FALSE;
						esc_flag = TRUE;
						if(n_message == MASSAGE_1)
						{
							for(addres_aux = BIT0;addres_aux < TXT_SIZE;addres_aux++)
							{
								UART_putChar(UART_0, buffer[addres_aux]);
							}
						}
						if(n_message == MASSAGE_2)
						{
							for(addres_aux = BIT0;addres_aux < TXT_SIZE;addres_aux++)
							{
								UART_putChar(UART_0, buffer[addres_aux]);
							}
						}
						if(n_message == MASSAGE_3)
						{
							for(addres_aux = BIT0;addres_aux < TXT_SIZE;addres_aux++)
							{
								UART_putChar(UART_0, buffer[addres_aux]);
							}
						}
						if(n_message == MASSAGE_4)
						{
							for(addres_aux = BIT0;addres_aux < TXT_SIZE;addres_aux++)
							{
								UART_putChar(UART_0, buffer[addres_aux]);
							}
						}
						if(n_message == MASSAGE_5)
						{
							for(addres_aux = BIT0;addres_aux < TXT_SIZE;addres_aux++)
							{
								UART_putChar(UART_0, buffer[addres_aux]);
							}
						}
					}
				}
			}
			if ((TRUE == get_esc()) || (FALSE == state_flag)) {
				TERA_menu();
				enter_flag = FALSE;
				esc_flag = FALSE;
				UART0_MailBox.flag = FALSE;
			}
			UART0_MailBox.flag = FALSE;
	}
	/*******************/
}

void set_key(uint8 key)
{
	key_flag = key;
}
uint8 get_key()
{
	return key_flag;
}
void set_enter(uint8 enter)
{
	if(enter == ASCII_ENTER)
		enter_flag = TRUE;
}
uint8 get_enter()
{
	return enter_flag;
}
void set_esc(uint8 esc)
{
	if(esc == ASCII_ESC)
		esc_flag = TRUE;
}
uint8 get_esc()
{
	return esc_flag;
}
