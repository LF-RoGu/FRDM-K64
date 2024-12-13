/**
	\file 	  MCP7940N.c
	\brief
			  This is the source file for the MCP7940N module. It contains functions
			  for the operation of the MCP7940N integrated circuit, a real time clock.
	\authors: César Villarreal Hernández, ie707560
	          José Luis Rodríguez Gutierrez,ie705694
	\date	  10/05/2019

	\TODO: IMPLEMENT I2C WITHOUT DELAYS: DMA? ESTATE MACHINES?
 */

#include "MCP7940N.h"

static uint8_t global_sec_data;
static uint8_t global_min_data;
static uint8_t global_hour_data;

static const MCP7940N_state_t FSM_Moore_MCP7940N[3]=
{
	{I2C_TX,    MCP7940N_WRITE_CONTROL,  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {MCP7940N_S1, MCP7940N_S2, MCP7940N_S0}}, //WRITE CONTROL STATE
    {I2C_TX,    ENABLE_ADDR,             I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {MCP7940N_S2, MCP7940N_S0, MCP7940N_S1}}, //SET ADDRESS STATE
	{I2C_TX,    ENABLE_COUNT,            I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {MCP7940N_S0, MCP7940N_S1, MCP7940N_S2}} //WRITE DATA STATE
};

static const MCP7940N_state_t FSM_Moore_time_addr[3]=
{
	{I2C_TX,    SEC_ADDR,                I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {MCP7940N_S1, MCP7940N_S2, MCP7940N_S0}}, //WRITE CONTROL STATE
    {I2C_TX,    MIN_ADDR,                I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {MCP7940N_S2, MCP7940N_S0, MCP7940N_S1}}, //SET ADDRESS STATE
	{I2C_TX,    HOUR_ADDR,               I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {MCP7940N_S0, MCP7940N_S1, MCP7940N_S2}} //WRITE DATA STATE
};

static const MCP7940N_time_state_t FSM_Moore_time_val[3]=
{
	{I2C_TX,    &global_sec_data,  I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {MCP7940N_S1, MCP7940N_S2, MCP7940N_S0}}, //WRITE CONTROL STATE
    {I2C_TX,    &global_min_data,                I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {MCP7940N_S2, MCP7940N_S0, MCP7940N_S1}}, //SET ADDRESS STATE
	{I2C_TX,    &global_hour_data,        I2C_tx_rx_mode, I2C_write_byte, I2C_wait, I2C_get_ack, I2C_start, I2C_stop,  {MCP7940N_S0, MCP7940N_S1, MCP7940N_S2}} //WRITE DATA STATE
};

void MCP7940N_enable()
{
	uint8_t	current_state;
	uint8_t tx_or_rx;
	uint8_t writebyte;

	/** Set current state to first command **/
	current_state = MCP7940N_S0;
	/** Enable transmission **/
	tx_or_rx = FSM_Moore_MCP7940N[current_state].Transmit_Receive;
	FSM_Moore_MCP7940N[current_state].fptrTxRx(tx_or_rx); //Enable transmission
	do
	{
		if(MCP7940N_S0 == current_state)
		{
			/** Generate start signal **/
			FSM_Moore_MCP7940N[current_state].start();
		}
		else
		{
		}

		writebyte = FSM_Moore_MCP7940N[current_state].writebyte;		//Retrieve current writebyte value
		FSM_Moore_MCP7940N[current_state].fptrWrite(writebyte);		//Send writebyte current value
		FSM_Moore_MCP7940N[current_state].fptrWaitTransfer();		//wait until transfer is complete
		FSM_Moore_MCP7940N[current_state].fptrgetAck();				//receive acknowledge
		current_state = FSM_Moore_MCP7940N[current_state].next[0];	//get next state

	}while(MCP7940N_S0 != current_state);
	FSM_Moore_MCP7940N[current_state].stop(); //stop transmission
}

void MCP7940N_set_sec(uint8_t data)
{
	uint8_t	current_state;
	uint8_t tx_or_rx;
	uint8_t writebyte;

	global_sec_data = data;
	/** Set current state to first command **/
	current_state = MCP7940N_S0;
	/** Enable transmission **/

	tx_or_rx = FSM_Moore_time_val[current_state].Transmit_Receive;
	FSM_Moore_time_val[MCP7940N_S0].fptrTxRx(tx_or_rx); //Enable transmission
	FSM_Moore_time_val[MCP7940N_S0].start();

	writebyte = FSM_Moore_MCP7940N[current_state].writebyte;		//Retrieve current writebyte value
	FSM_Moore_MCP7940N[MCP7940N_S0].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore_MCP7940N[MCP7940N_S0].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore_MCP7940N[MCP7940N_S0].fptrgetAck();				//receive acknowledge

	writebyte = FSM_Moore_time_addr[current_state].writebyte;		//Retrieve current writebyte value
	FSM_Moore_time_addr[current_state].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore_time_addr[current_state].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore_time_addr[current_state].fptrgetAck();				//receive acknowledge

	writebyte = *FSM_Moore_time_val[current_state].writebyte;		//Retrieve current writebyte value
	FSM_Moore_time_val[current_state].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore_time_val[current_state].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore_time_val[current_state].fptrgetAck();				//receive acknowledge

	FSM_Moore_MCP7940N[MCP7940N_S0].stop(); //stop transmission
}

void MCP7940N_set_min(uint8_t data)
{
	uint8_t	current_state;
	uint8_t tx_or_rx;
	uint8_t writebyte;

	global_min_data = data;
	/** Set current state to first command **/
	current_state = MCP7940N_S1;
	/** Enable transmission **/

	tx_or_rx = FSM_Moore_time_val[MCP7940N_S0].Transmit_Receive;
	FSM_Moore_time_val[MCP7940N_S0].fptrTxRx(tx_or_rx); //Enable transmission
	FSM_Moore_time_val[MCP7940N_S0].start();

	writebyte = FSM_Moore_MCP7940N[MCP7940N_S0].writebyte;		//Retrieve current writebyte value
	FSM_Moore_MCP7940N[MCP7940N_S0].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore_MCP7940N[MCP7940N_S0].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore_MCP7940N[MCP7940N_S0].fptrgetAck();				//receive acknowledge

	writebyte = FSM_Moore_time_addr[current_state].writebyte;		//Retrieve current writebyte value
	FSM_Moore_time_addr[current_state].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore_time_addr[current_state].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore_time_addr[current_state].fptrgetAck();				//receive acknowledge

	writebyte = *FSM_Moore_time_val[current_state].writebyte;		//Retrieve current writebyte value
	FSM_Moore_time_val[current_state].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore_time_val[current_state].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore_time_val[current_state].fptrgetAck();				//receive acknowledge

	FSM_Moore_MCP7940N[MCP7940N_S0].stop(); //stop transmission
}

void MCP7940N_set_hour(uint8_t data)
{
	uint8_t	current_state;
	uint8_t tx_or_rx;
	uint8_t writebyte;

	global_hour_data = data;
	/** Set current state to first command **/
	current_state = MCP7940N_S2;
	/** Enable transmission **/

	tx_or_rx = FSM_Moore_time_val[MCP7940N_S0].Transmit_Receive;
	FSM_Moore_time_val[MCP7940N_S0].fptrTxRx(tx_or_rx); //Enable transmission
	FSM_Moore_time_val[MCP7940N_S0].start();

	writebyte = FSM_Moore_MCP7940N[MCP7940N_S0].writebyte;		//Retrieve current writebyte value
	FSM_Moore_MCP7940N[MCP7940N_S0].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore_MCP7940N[MCP7940N_S0].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore_MCP7940N[MCP7940N_S0].fptrgetAck();				//receive acknowledge

	writebyte = FSM_Moore_time_addr[current_state].writebyte;		//Retrieve current writebyte value
	FSM_Moore_time_addr[current_state].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore_time_addr[current_state].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore_time_addr[current_state].fptrgetAck();				//receive acknowledge

	writebyte = *FSM_Moore_time_val[current_state].writebyte;		//Retrieve current writebyte value
	FSM_Moore_time_val[current_state].fptrWrite(writebyte);		//Send writebyte current value
	FSM_Moore_time_val[current_state].fptrWaitTransfer();		//wait until transfer is complete
	FSM_Moore_time_val[current_state].fptrgetAck();				//receive acknowledge

	FSM_Moore_MCP7940N[MCP7940N_S0].stop(); //stop transmission
}

uint8_t MCP7940N_get_sec()
{
	uint8_t data;
	/*Send the Tx mode
	 * Start bit*/
	I2C_start();
	/*Send the MCP7940N Address to the register*/
	I2C_write_byte(MCP7940N_WRITE_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Register address*/
	I2C_write_byte(SEC_ADDR);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the start bit signal again so we can send now the data to read*/
	I2C_repeated_start();

	/*Writing to the slave to read the previous register*/
	I2C_write_byte(MCP7940N_READ_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Chance I2C module to Rx mode*/
	I2C_tx_rx_mode(I2C_RX);

	/*Generate ~Acknowledge*/
	I2C_nack();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();
	/*Check if I2C is busy*/
	I2C_wait();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Generate ~Acknowledge*/
	I2C_nack();
	/*Send the stop signal*/
	I2C_stop();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();

	data = (data & 0x7F);

	return (data);
}

uint8_t MCP7940N_get_min(void)
{
	uint8_t data;
	/*Send the Tx mode
	 * Start bit*/
	I2C_start();
	/*Send the MCP7940N Address to the register*/
	I2C_write_byte(MCP7940N_WRITE_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Register address*/
	I2C_write_byte(MIN_ADDR);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the start bit signal again so we can send now the data to read*/
	I2C_repeated_start();

	/*Writing to the slave to read the previous register*/
	I2C_write_byte(MCP7940N_READ_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Chance I2C module to Rx mode*/
	I2C_tx_rx_mode(I2C_RX);

	/*Generate ~Acknowledge*/
	I2C_nack();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();
	/*Check if I2C is busy*/
	I2C_wait();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the stop signal*/
	I2C_stop();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();

	return (data);
}

uint8_t MCP7940N_get_hour(void)
{
	uint8_t data;
	/*Send the Tx mode
	 * Start bit*/
	I2C_start();
	/*Send the MCP7940N Address to the register*/
	I2C_write_byte(MCP7940N_WRITE_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Register address*/
	I2C_write_byte(HOUR_ADDR);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the start bit signal again so we can send now the data to read*/
	I2C_repeated_start();

	/*Writing to the slave to read the previous register*/
	I2C_write_byte(MCP7940N_READ_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Chance I2C module to Rx mode*/
	I2C_tx_rx_mode(I2C_RX);

	/*Generate ~Acknowledge*/
	I2C_nack();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();
	/*Check if I2C is busy*/
	I2C_wait();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the stop signal*/
	I2C_stop();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();

	return (data);
}

void MCP7940N_set_date(uint8_t data)
{
	/*Start bit*/
	I2C_start();
	/*Send the MCP7940N Address to the register*/
	I2C_write_byte(MCP7940N_WRITE_CONTROL); /*1101/Direccion fisica(A2/A1/A0)*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Register address*/
	I2C_write_byte(DATE_ADDR);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*data code*/
	I2C_write_byte(data); /*Dato que se envia*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the stop signal*/
	I2C_stop();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);
}

uint8_t MCP7940N_get_date(void)
{
	uint8_t data;
	/*Send the Tx mode
	 * Start bit*/
	I2C_start();
	/*Send the MCP7940N Address to the register*/
	I2C_write_byte(MCP7940N_WRITE_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Register address*/
	I2C_write_byte(DATE_ADDR);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the start bit signal again so we can send now the data to read*/
	I2C_repeated_start();

	/*Writing to the slave to read the previous register*/
	I2C_write_byte(MCP7940N_READ_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Chance I2C module to Rx mode*/
	I2C_tx_rx_mode(I2C_RX);

	/*Generate ~Acknowledge*/
	I2C_nack();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();
	/*Check if I2C is busy*/
	I2C_wait();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the stop signal*/
	I2C_stop();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();

	return (data);
}

void MCP7940N_set_month(uint8_t data)
{
	/*Start bit*/
	I2C_start();
	/*Send the MCP7940N Address to the register*/
	I2C_write_byte(MCP7940N_WRITE_CONTROL); /*1101/Direccion fisica(A2/A1/A0)*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Register address*/
	I2C_write_byte(MONTH_ADDR);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*data code*/
	I2C_write_byte(data); /*Dato que se envia*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the stop signal*/
	I2C_stop();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);
}
uint8_t MCP7940N_get_month(void)
{
	uint8_t data;
	/*Send the Tx mode
	 * Start bit*/
	I2C_start();
	/*Send the MCP7940N Address to the register*/
	I2C_write_byte(MCP7940N_WRITE_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Register address*/
	I2C_write_byte(MONTH_ADDR);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the start bit signal again so we can send now the data to read*/
	I2C_repeated_start();

	/*Writing to the slave to read the previous register*/
	I2C_write_byte(MCP7940N_READ_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Chance I2C module to Rx mode*/
	I2C_tx_rx_mode(I2C_RX);

	/*Generate ~Acknowledge*/
	I2C_nack();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();
	/*Check if I2C is busy*/
	I2C_wait();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the stop signal*/
	I2C_stop();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();

	return (data);
}
void MCP7940N_set_year(uint8_t data)
{
	/*Start bit*/
	I2C_start();
	/*Send the MCP7940N Address to the register*/
	I2C_write_byte(MCP7940N_WRITE_CONTROL); /*1101/Direccion fisica(A2/A1/A0)*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Register address*/
	I2C_write_byte(YEAR_ADDR);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*data code*/
	I2C_write_byte(data); /*Dato que se envia*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the stop signal*/
	I2C_stop();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);
}
uint8_t MCP7940N_get_year(void)
{
	uint8_t data;
	/*Send the Tx mode
	 * Start bit*/
	I2C_start();
	/*Send the MCP7940N Address to the register*/
	I2C_write_byte(MCP7940N_WRITE_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Register address*/
	I2C_write_byte(YEAR_ADDR);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the start bit signal again so we can send now the data to read*/
	I2C_repeated_start();

	/*Writing to the slave to read the previous register*/
	I2C_write_byte(MCP7940N_READ_CONTROL);
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Chance I2C module to Rx mode*/
	I2C_tx_rx_mode(I2C_RX);

	/*Generate ~Acknowledge*/
	I2C_nack();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();
	/*Check if I2C is busy*/
	I2C_wait();
	/*Delay to secure we get the signal*/
	delay(MCP7940N_DELAY);

	/*Send the stop signal*/
	I2C_stop();
	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();

	return (data);
}
