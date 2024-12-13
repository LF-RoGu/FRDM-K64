/**
	\file 	  HT16K33.c
	\brief
			  This is the source file for the matrix module. It contains functions
			  for the operation of the M1707042 8*8 LED matrix.
	\authors: César Villarreal Hernández, ie707560
	          José Luis Rodríguez Gutierrez,ie705694
	\date	  28/04/2019
	Graphics created using: http://robojax.com/learn/arduino/8x8LED/
 */

#include <HT16K33.h>
#include "displayArr.c"

static led_HT16K33_t global_matrix = {0};

/** Table containing the information that relates row values and numeric data **/
static time_display_t display_info[TIME_HT16K33_TABLE_SIZE] =
{
    {ZERO,   	  0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b01111110,  0b01000010,  0b01111110},
    {ONE,    	  0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b00000000,  0b01111110,  0b00000000},
    {TWO,    	  0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b01011110,  0b01010010,  0b01110010},
    {THREE,  	  0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b01010010,  0b01010010,  0b01111110}, 
    {FOUR,   	  0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b01110000,  0b00010000,  0b01111110},
    {FIVE,   	  0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b01110010,  0b01010010,  0b01011110},
    {SIX,    	  0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b01111110,  0b01001010,  0b01001110},
    {SEVEN,  	  0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b01000000,  0b01000000,  0b01111110},
    {EIGHT,  	  0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b01111110,  0b01010010,  0b01111110},
    {NINE,        0b00000000,  0b01111110,  0b01000010,  0b01111110,  0b00000000,  0b01110000,  0b01010000,  0b01111110},
    {TEN,         0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b01111110,  0b01000010,  0b01111110},
    {ELEVEN,      0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b00000000,  0b01111110,  0b00000000},
    {TWELVE,      0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b01011110,  0b01010010,  0b01110010},
	{THIRTEEN,    0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b01010010,  0b01010010,  0b01111110},
	{FOURTEEN,    0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b01110000,  0b00010000,  0b01111110},
	{FIFTHTEEN,   0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b01110010,  0b01010010,  0b01011110},
	{SIXTEEN,     0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b01111110,  0b01001010,  0b01001110},
	{SEVENTEEN,   0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b01000000,  0b01000000,  0b01111110},
	{EIGHTEEN,    0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b01111110,  0b01010010,  0b01111110},
	{NINETEEN,    0b00000000,  0b00000000,  0b01111110,  0b00000000,  0b00000000,  0b01110000,  0b01010000,  0b01111110},
	{TWENTY,      0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b01111110,  0b01000010,  0b01111110},
	{TWENTYONE,   0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b00000000,  0b01111110,  0b00000000},
	{TWENTYTWO,   0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b01011110,  0b01010010,  0b01110010},
	{TWENTYTHREE, 0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b01010010,  0b01010010,  0b01111110},
	{TWENTYFOUR,  0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b01110000,  0b00010000,  0b01111110},
	{TWENTYFIVE,  0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b01110010,  0b01010010,  0b01011110},
	{TWENTYSIX,   0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b01111110,  0b01001010,  0b01001110},
	{TWENTYSEVEN, 0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b01000000,  0b01000000,  0b01111110},
	{TWENTYEIGHT, 0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b01111110,  0b01010010,  0b01111110},
	{TWENTYNINE,  0b00000000,  0b01011110,  0b01010010,  0b01110010,  0b00000000,  0b01110000,  0b01010000,  0b01111110},
	{THIRTY, 	  0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b01111110,  0b01000010,  0b01111110},
	{THIRTYONE,   0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b00000000,  0b01111110,  0b00000000},
	{THIRTYTWO,   0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b01011110,  0b01010010,  0b01110010},
	{THIRTYTHREE, 0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b01010010,  0b01010010,  0b01111110},
	{THIRTYFOUR,  0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b01110000,  0b00010000,  0b01111110},
	{THIRTYFIVE,  0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b01110010,  0b01010010,  0b01011110},
	{THIRTYSIX,   0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b01111110,  0b01001010,  0b01001110},
	{THIRTYSEVEN, 0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b01000000,  0b01000000,  0b01111110},
	{THIRTYEIGHT, 0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b01111110,  0b01010010,  0b01111110},
	{THIRTYNINE,  0b00000000,  0b01010010,  0b01010010,  0b01111110,  0b00000000,  0b01110000,  0b01010000,  0b01111110},
	{FOURTY, 	  0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b01111110,  0b01000010,  0b01111110},
	{FOURTYONE,   0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b00000000,  0b01111110,  0b00000000},
	{FOURTYTWO,   0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b01011110,  0b01010010,  0b01110010},
	{FOURTYTHREE, 0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b01010010,  0b01010010,  0b01111110},
	{FOURTYFOUR,  0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b01110000,  0b00010000,  0b01111110},
	{FOURTYFIVE,  0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b01110010,  0b01010010,  0b01011110},
	{FOURTYSIX,   0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b01111110,  0b01001010,  0b01001110},
	{FOURTYSEVEN, 0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b01000000,  0b01000000,  0b01111110},
	{FOURTYEIGHT, 0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b01111110,  0b01010010,  0b01111110},
	{FOURTYNINE,  0b00000000,  0b01110000,  0b00010000,  0b01111110,  0b00000000,  0b01110000,  0b01010000,  0b01111110},
	{FIFTY, 	  0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b01111110,  0b01000010,  0b01111110},
	{FIFTYONE,    0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b00000000,  0b01111110,  0b00000000},
	{FIFTYTWO,    0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b01011110,  0b01010010,  0b01110010},
	{FIFTYTHREE,  0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b01010010,  0b01010010,  0b01111110},
	{FIFTYFOUR,   0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b01110000,  0b00010000,  0b01111110},
	{FIFTYFIVE,   0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b01110010,  0b01010010,  0b01011110},
	{FIFTYSIX,    0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b01111110,  0b01001010,  0b01001110},
	{FIFTYSEVEN,  0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b01000000,  0b01000000,  0b01111110},
	{FIFTYEIGHT,  0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b01111110,  0b01010010,  0b01111110},
	{FIFTYNINE,   0b00000000,  0b01110010,  0b01010010,  0b01011110,  0b00000000,  0b01110000,  0b01010000,  0b01111110}
};

/** Table contains data for animations **/
static time_display_t display_startup[1] =
{
	{SQ_START,    0b11111111,  0b10000001,  0b10111101,  0b10100101,  0b10100101,  0b10111101,  0b10000001,  0b11111111}
};

const I2C_state_t FSM_Moore[5]=
{
	{I2C_TX,   HT16K33_WRITE_CONTROL,  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S0}}, //WRITE CONTROL STATE
    {I2C_TX,    HT16K33_OSC_CONTROL1,  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S0, HT16K33_I2C_S1}}, //SET ADDRESS STATE
	{I2C_TX,    HT16K33_OSC_CONTROL2,  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S3, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2}}, //WRITE DATA STATE
	{I2C_TX,    HT16K33_OSC_CONTROL3,  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3}}, //WRITE DATA STATE
	{I2C_TX,         HT16K33_ADDRESS,  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3}} //WRITE DATA STATE
};

const I2C_D_state_t FSM_Display1[8]=
{
    {I2C_TX,   &global_matrix.led_display_1[row_0],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1}},
	{I2C_TX,   &global_matrix.led_display_1[row_1],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2}},
	{I2C_TX,   &global_matrix.led_display_1[row_2],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3}},
	{I2C_TX,   &global_matrix.led_display_1[row_3],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4}},
	{I2C_TX,   &global_matrix.led_display_1[row_4],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5}},
	{I2C_TX,   &global_matrix.led_display_1[row_5],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6}},
	{I2C_TX,   &global_matrix.led_display_1[row_6],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7}},
	{I2C_TX,   &global_matrix.led_display_1[row_7],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S8}}
};

const I2C_D_state_t FSM_Display2[8]=
{
    {I2C_TX,   &global_matrix.led_display_2[row_0],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1}},
	{I2C_TX,   &global_matrix.led_display_2[row_1],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2}},
	{I2C_TX,   &global_matrix.led_display_2[row_2],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3}},
	{I2C_TX,   &global_matrix.led_display_2[row_3],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4}},
	{I2C_TX,   &global_matrix.led_display_2[row_4],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5}},
	{I2C_TX,   &global_matrix.led_display_2[row_5],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6}},
	{I2C_TX,   &global_matrix.led_display_2[row_6],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S7, HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7}},
	{I2C_TX,   &global_matrix.led_display_2[row_7],  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {HT16K33_I2C_S0, HT16K33_I2C_S1, HT16K33_I2C_S2, HT16K33_I2C_S3, HT16K33_I2C_S4, HT16K33_I2C_S5, HT16K33_I2C_S6, HT16K33_I2C_S7, HT16K33_I2C_S8}}
};


void HT16K33_init(void)
{
	uint8_t	current_state;
	uint8_t tx_or_rx;
	uint8_t writebyte;

	/** Set current state to first command **/
	current_state = HT16K33_I2C_S1;
	/** Enable transmission **/
	tx_or_rx = FSM_Moore[current_state].Transmit_Receive;
	FSM_Moore[current_state].fptrTxRx(tx_or_rx); //Enable transmission

	do
	{
		if(HT16K33_I2C_S1 == current_state)
		{
			/** Generate start signal **/
			FSM_Moore[current_state].start();
		}
		else
		{
			/** Generate repeated start signal **/
			I2C_repeated_start();
		}

		writebyte = FSM_Moore[HT16K33_I2C_S0].writebyte;		//Retrieve current writebyte value
		FSM_Moore[HT16K33_I2C_S0].fptrWrite(writebyte);		//Send writebyte current value
		FSM_Moore[HT16K33_I2C_S0].fptrWaitTransfer();		//wait until transfer is complete
		FSM_Moore[HT16K33_I2C_S0].fptrgetAck();				//receive acknowledge

		writebyte = FSM_Moore[current_state].writebyte;		//Retrieve current writebyte value
		FSM_Moore[current_state].fptrWrite(writebyte);		//Send writebyte current value
		FSM_Moore[current_state].fptrWaitTransfer();		//wait until transfer is complete
		FSM_Moore[current_state].fptrgetAck();				//receive acknowledge
		current_state = FSM_Moore[current_state].next[0];	//get next state

	}while(HT16K33_I2C_S0 != current_state);

	FSM_Moore[current_state].stop(); //stop transmission
}

void HT16K33_data_transmit()
{
	uint8_t	current_state;
	uint8_t tx_or_rx;
	uint8_t writebyte;

	tx_or_rx = FSM_Moore[HT16K33_I2C_S0].Transmit_Receive;
	current_state = HT16K33_I2C_S0;

	FSM_Moore[HT16K33_I2C_S0].fptrTxRx(tx_or_rx); //Enable transmission
	FSM_Moore[HT16K33_I2C_S0].start();

	writebyte = FSM_Moore[HT16K33_I2C_S0].writebyte;		//Retrieve current writebyte value
	FSM_Moore[HT16K33_I2C_S0].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore[HT16K33_I2C_S0].fptrWaitTransfer();		//wait until transfer is complete

	writebyte = FSM_Moore[HT16K33_I2C_S4].writebyte;		//Retrieve current writebyte value
	FSM_Moore[HT16K33_I2C_S4].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore[HT16K33_I2C_S4].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore[HT16K33_I2C_S4].fptrgetAck();				//receive acknowledge

	do
	{
		writebyte = *FSM_Display1[current_state].writebyte;		//Retrieve current writebyte value
		FSM_Display1[current_state].fptrWrite(writebyte);		//Send writebyte current value
		FSM_Display1[current_state].fptrWaitTransfer();		    //wait until transfer is complete
		FSM_Display1[current_state].fptrgetAck();				//receive acknowledge

		writebyte = *FSM_Display2[current_state].writebyte;		//Retrieve current writebyte value
		FSM_Display2[current_state].fptrWrite(writebyte);	 	//Send writebyte current value
		FSM_Display2[current_state].fptrWaitTransfer();		    //wait until transfer is complete
		FSM_Display2[current_state].fptrgetAck();				//receive acknowledge
		current_state = FSM_Display2[current_state].next[0];	//get next state

	}while(HT16K33_I2C_S0 != current_state);

	FSM_Moore[HT16K33_I2C_S0].stop();
}

void HT16K33_set_txt(uint8_t txt_symbol, uint8_t ndisplay)
{
	//look up for text symbol in table
	switch(ndisplay)
	{
		case FIRST_DISPLAY:
		 		/** look up for text symbol in table and update global matrix **/
				HT16K33_get_sprite_data(txt_symbol, FIRST_DISPLAY);
				/** Show global matrix on first LCD display global matrix **/
				HT16K33_data_transmit();
		break;
		case SECOND_DISPLAY:
				/** look up for text symbol in table and update global matrix **/
			    HT16K33_get_sprite_data(txt_symbol, SECOND_DISPLAY);
				/** Show global matrix on second LCD display global matrix **/
				HT16K33_data_transmit();
		break;
		default:
		break;
	}
}

void HT16K33_get_sprite_data(uint8_t value, uint8_t ndisplay)
{
    uint8_t i;
    uint8_t index;
    boolean_t value_found_flag;

    value_found_flag = 0;

    for(i = 0; TIME_HT16K33_TABLE_SIZE > i ; i++)
    {
		/** Verify if the value given corresponds to any data on the table **/
        if(display_info[i].value == value)
        {
            index = i; //set the index corresponding to the value
            value_found_flag = TRUE; //indicate that a value has been found
            break; //end loop
        }
        else
        {
            continue; //do-nothing
        }
    }

    /** If the index is valid, it returns the display information **/
    if(TRUE == value_found_flag)
    {
		/** Set the rows corresponding to the value found on the table **/
		switch(ndisplay)
		{
			case FIRST_DISPLAY:
				global_matrix.led_display_1[row_0] = display_info[index].row_0;
				global_matrix.led_display_1[row_1] = display_info[index].row_1;
				global_matrix.led_display_1[row_2] = display_info[index].row_2;
				global_matrix.led_display_1[row_3] = display_info[index].row_3;
				global_matrix.led_display_1[row_4] = display_info[index].row_4;
				global_matrix.led_display_1[row_5] = display_info[index].row_5;
				global_matrix.led_display_1[row_6] = display_info[index].row_6;
				global_matrix.led_display_1[row_7] = display_info[index].row_7;
			break;
			case SECOND_DISPLAY:
				global_matrix.led_display_2[row_0] = display_info[index].row_0;
				global_matrix.led_display_2[row_1] = display_info[index].row_1;
				global_matrix.led_display_2[row_2] = display_info[index].row_2;
				global_matrix.led_display_2[row_3] = display_info[index].row_3;
				global_matrix.led_display_2[row_4] = display_info[index].row_4;
				global_matrix.led_display_2[row_5] = display_info[index].row_5;
				global_matrix.led_display_2[row_6] = display_info[index].row_6;
				global_matrix.led_display_2[row_7] = display_info[index].row_7;
			break;
			default:
			break;
		}
    }
}

void HT16K33_DOT(void)
{
	I2C_tx_rx_mode(I2C_TX);
	I2C_start();
	I2C_write_byte(HT16K33_WRITE_CONTROL);
	I2C_wait();

	I2C_write_byte(HT16K33_ADDRESS);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(HT16K33_DOT_MASK);
	I2C_wait();
	I2C_get_ack();

	I2C_stop();
}

void HT16K33_STARTUP()
{
	global_matrix.led_display_1[row_0] = display_startup[row_0].row_0;
	global_matrix.led_display_1[row_1] = display_startup[row_0].row_1;
	global_matrix.led_display_1[row_2] = display_startup[row_0].row_2;
	global_matrix.led_display_1[row_3] = display_startup[row_0].row_3;
	global_matrix.led_display_1[row_4] = display_startup[row_0].row_4;
	global_matrix.led_display_1[row_5] = display_startup[row_0].row_5;
	global_matrix.led_display_1[row_6] = display_startup[row_0].row_6;
	global_matrix.led_display_1[row_7] = display_startup[row_0].row_7;

	global_matrix.led_display_2[row_0] = display_startup[row_0].row_0;
	global_matrix.led_display_2[row_1] = display_startup[row_0].row_1;
	global_matrix.led_display_2[row_2] = display_startup[row_0].row_2;
	global_matrix.led_display_2[row_3] = display_startup[row_0].row_3;
	global_matrix.led_display_2[row_4] = display_startup[row_0].row_4;
	global_matrix.led_display_2[row_5] = display_startup[row_0].row_5;
	global_matrix.led_display_2[row_6] = display_startup[row_0].row_6;
	global_matrix.led_display_2[row_7] = display_startup[row_0].row_7;

	HT16K33_data_transmit();
}

void MATRIX_word_transmit(int8_t x[8][8], int8_t y[8][8])
{
	/*Set as Tx*/
	I2C_tx_rx_mode(I2C_TX);
	/*Start bit*/
	I2C_start();
	/*Send the RTC Address to the register*/
	I2C_write_byte(HT16K33_WRITE_CONTROL); /*1010/Direccion fisica(A2/A1/A0)*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*delay*/
	delay(HT16K33_DELAY);

	/*Register address*/
	I2C_write_byte(HT16K33_ADDRESS); /*Address = data*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	delay(HT16K33_DELAY);

	for(uint8_t index = 0; index < 8; index++)
	{
		for(uint8_t j = 0;j < 8;j++)
		{
			/*Register address*/
			I2C_write_byte(x[index][j]); /*Address = data*/
			/*Check if I2C is busy*/
			I2C_wait();
			/*Recevie the Acknowledge*/
			I2C_get_ack();
			/*delay*/
			delay(HT16K33_DELAY);
	/***********************************************************/
			/*Register address*/
			I2C_write_byte(y[index][j]); /*Address = data*/
			/*Check if I2C is busy*/
			I2C_wait();
			/*Recevie the Acknowledge*/
			I2C_get_ack();
			/*delay*/
			delay(HT16K33_DELAY);

			sleep_sec();
		}
	}

	/*Send the stop signal*/
	I2C_stop();
}

void MATRIX_set_txt(int8_t* txt)
{
	uint8_t index = 0;
	uint8_t txt_arr[50][8][8] = {0};

	for(uint8_t i = 0;i<20;i++)
	{
		switch(txt[i])
		{
		case 'a':
		case 'A':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_A[i][j];
							}
						}
						index++;
				break;
		case 'b':
		case 'B':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_B[i][j];
							}
						}
						index++;
				break;
		case 'c':
		case 'C':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_C[i][j];
							}
						}
						index++;
				break;
		case 'd':
		case 'D':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_D[i][j];
							}
						}
						index++;
				break;
		case 'e':
		case 'E':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_E[i][j];
							}
						}
						index++;
				break;
		case 'f':
		case 'F':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_F[i][j];
							}
						}
						index++;
				break;
		case 'g':
		case 'G':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_G[i][j];
							}
						}
						index++;
				break;
		case 'h':
		case 'H':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_H[i][j];
							}
						}
						index++;
				break;
		case 'i':
		case 'I':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_I[i][j];
							}
						}
						index++;
				break;
		case 'j':
		case 'J':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_J[i][j];
							}
						}
						index++;
				break;
		case 'k':
		case 'K':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_K[i][j];
							}
						}
						index++;
				break;
		case 'l':
		case 'L':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_L[i][j];
							}
						}
						index++;
				break;
		case 'm':
		case 'M':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_M[i][j];
							}
						}
						index++;
				break;
		case 'n':
		case 'N':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_N[i][j];
							}
						}
						index++;
				break;
		case 'o':
		case 'O':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_O[i][j];
							}
						}
						index++;
				break;
		case 'p':
		case 'P':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_P[i][j];
							}
						}
						index++;
				break;
		case 'q':
		case 'Q':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_Q[i][j];
							}
						}
						index++;
				break;
		case 'r':
		case 'R':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_R[i][j];
							}
						}
						index++;
				break;
		case 's':
		case 'S':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_S[i][j];
							}
						}
						index++;
				break;
		case 't':
		case 'T':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_T[i][j];
							}
						}
						index++;
				break;
		case 'u':
		case 'U':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_U[i][j];
							}
						}
						index++;
				break;
		case 'v':
		case 'V':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_V[i][j];
							}
						}
						index++;
				break;
		case 'w':
		case 'W':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_W[i][j];
							}
						}
						index++;
				break;
		case 'x':
		case 'X':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_X[i][j];
							}
						}
						index++;
				break;
		case 'y':
		case 'Y':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_Y[i][j];
							}
						}
						index++;
				break;
		case 'z':
		case 'Z':
						for(uint8_t i = 0; i < 8;i++)
						{
							for(uint8_t j = 0; j < 8;j++)
							{
								txt_arr[index][i][j] = data_Z[i][j];
							}
						}
						index++;
				break;
		case '1':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_One[i][j];
				}
			}
			index++;
				break;
		case '2':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Two[i][j];
				}
			}
			index++;
				break;
		case '3':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Three[i][j];
				}
			}
			index++;
				break;
		case '4':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Four[i][j];
				}
			}
			index++;
				break;
		case '5':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Five[i][j];
				}
			}
			index++;
				break;
		case '6':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Six[i][j];
				}
			}
			index++;
				break;
		case '7':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Seven[i][j];
				}
			}
			index++;
				break;
		case '8':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Eight[i][j];
				}
			}
			index++;
				break;
		case '9':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Nine[i][j];
				}
			}
			index++;
				break;
		case '0':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Zero[i][j];
				}
			}
			index++;
				break;
		case ':':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_Time[i][j];
				}
			}
			index++;
				break;
		case ' ':
			for(uint8_t i = 0; i < 8;i++)
			{
				for(uint8_t j = 0; j < 8;j++)
				{
					txt_arr[index][i][j] = data_0[i][j];
				}
			}
			index++;
				break;
		default:
				break;

		}
	}
	/**/
	index = 0;

	for(uint8_t snd_index = 0; snd_index<10;snd_index++)
	{
		MATRIX_word_transmit(txt_arr[snd_index], txt_arr[snd_index+1]);
	}
}
