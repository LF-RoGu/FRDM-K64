/*
 * EEPROM.c
 *
 *  Created on: 03/11/2018
 *      Author: LuisFernando
 */

#include "EEPROM.h"

static sint8 mem_data[PAGE_SIZE] = {FALSE};

void EEPROM_write_mem(uint16 address,sint8 data)
{
	/*Need to separate address to High and Low address*/
	/*high address*/
	uint8 Haddr = address >> BIT8;
	/*low address*/
	uint8 Laddr =  address;

	/*Set as Tx*/
	I2C_tx_rx_mode(I2C_TX);
	/*Start bit*/
	I2C_start();
	/*Send the RTC Address to the register*/
	I2C_write_byte(EEPROM_WRITE_CONTROL); /*1010/Direccion fisica(A2/A1/A0)*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Register address*/
	I2C_write_byte(Haddr); /*Address = data*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Register address*/
	I2C_write_byte(Laddr); /*Address = data*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Register address*/
	I2C_write_byte(data); /*Address = data*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Send the stop signal*/
	I2C_stop();
}

void EEPROM_write_string_mem(uint16 address,sint8* ptr_data)
{
	/*Need to separate address to High and Low address*/
	/*high address*/
	uint8 Haddr = address >> BIT8;
	/*low address*/
	uint8 Laddr = address;

	/*variable used for the writing*/
	uint8 byte_control = FALSE;

	/*Set as Tx*/
	I2C_tx_rx_mode(I2C_TX);
	/*Start bit*/
	I2C_start();
	/*Send the RTC Address to the register*/
	I2C_write_byte(EEPROM_WRITE_CONTROL); /*1010/Direccion fisica(A2/A1/A0)*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Register address*/
	I2C_write_byte(Haddr); /*Address = data*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Register address*/
	I2C_write_byte(Laddr); /*Address = data*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Set a continues data write*/
	for(byte_control = FALSE; byte_control < PAGE_SIZE; byte_control++)
	{
		/*Register address*/
		I2C_write_byte(*ptr_data); /*Send to the register the value stored*/
		/*Check if I2C is busy*/
		I2C_wait();
		/*Recevie the Acknowledge*/
		I2C_get_ack();
		/*delay*/
		EEPROM_delay(EEPROM_DELAY);
		/*Increment ptr*/
		ptr_data++;
		if(NULL == ptr_data[byte_control])
			byte_control = PAGE_SIZE;
	}

	/*Send the stop signal*/
	I2C_stop();
}

sint8 EEPROM_read_mem(uint16 address)
{
	sint8 data;
	/*Need to separate address to High and Low address*/
	/*high address*/
	uint8 Haddr = address >> BIT8;
	/*low address*/
	uint8 Laddr = address;

	/*Change I2C module to Tx mode*/
	I2C_tx_rx_mode(I2C_TX);
	/*Start bit*/
	I2C_start();
	/*Send the RTC Address to the register*/
	I2C_write_byte(EEPROM_WRITE_CONTROL); /*1010/Direccion fisica(A2/A1/A0)*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Register address*/
	I2C_write_byte(Haddr); /*Address = data*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Register address*/
	I2C_write_byte(Laddr); /*Address = data*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Send the start bit signal again so we can send now the data to read*/
	I2C_repeted_start();

	/*Send the RTC Address to the register*/
	I2C_write_byte(EEPROM_READ_CONTROL); /*1010/Direccion fisica(A2/A1/A0)*/
	/*Check if I2C is busy*/
	I2C_wait();
	/*Recevie the Acknowledge*/
	I2C_get_ack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Change I2C module to Rx mode*/
	I2C_tx_rx_mode(I2C_RX);

	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();
	/*Check if I2C is busy*/
	I2C_wait();
	/*Generate ~Acknowledge*/
	I2C_nack();
	/*delay*/
	EEPROM_delay(EEPROM_DELAY);

	/*Send the stop signal*/
	I2C_stop();

	/*data get the value that's in the I2C_D register*/
	data = I2C_read_byte();

	return (data);
}
sint8* EEPROM_read_string_mem(uint16 address)
{
	uint8 counter = FALSE;
	/*Need to separate address to High and Low address*/
	/*high address*/
	uint8 Haddr = FALSE;
	/*low address*/
	uint8 Laddr = FALSE;

	do
	{
		Haddr = address >> BIT8;
		Laddr = address;

		/*Change I2C module to Tx mode*/
		I2C_tx_rx_mode(I2C_TX);
		/*Start bit*/
		I2C_start();
		/*Send the RTC Address to the register*/
		I2C_write_byte(EEPROM_WRITE_CONTROL); /*1010/Direccion fisica(A2/A1/A0)*/
		/*Check if I2C is busy*/
		I2C_wait();
		/*Recevie the Acknowledge*/
		I2C_get_ack();
		/*delay*/
		EEPROM_delay(EEPROM_DELAY);

		/*Register address*/
		I2C_write_byte(Haddr); /*Address = data*/
		/*Check if I2C is busy*/
		I2C_wait();
		/*Recevie the Acknowledge*/
		I2C_get_ack();
		/*delay*/
		EEPROM_delay(EEPROM_DELAY);

		/*Register address*/
		I2C_write_byte(Laddr); /*Address = data*/
		/*Check if I2C is busy*/
		I2C_wait();
		/*Recevie the Acknowledge*/
		I2C_get_ack();
		/*delay*/
		EEPROM_delay(EEPROM_DELAY);

		/*Send the start bit signal again so we can send now the data to read*/
		I2C_repeted_start();

		/*Send the RTC Address to the register*/
		I2C_write_byte(EEPROM_READ_CONTROL); /*1010/Direccion fisica(A2/A1/A0)*/
		/*Check if I2C is busy*/
		I2C_wait();
		/*Recevie the Acknowledge*/
		I2C_get_ack();
		/*delay*/
		EEPROM_delay(EEPROM_DELAY);

		/*Change I2C module to Rx mode*/
		I2C_tx_rx_mode(I2C_RX);

		/*data get the value that's in the I2C_D register*/
		mem_data[counter] = I2C_read_byte();
		/*Check if I2C is busy*/
		I2C_wait();
		/*Generate ~Acknowledge*/
		I2C_nack();
		/*delay*/
		EEPROM_delay(EEPROM_DELAY);

		/*Send the stop signal*/
		I2C_stop();

		/*data get the value that's in the I2C_D register*/
		mem_data[counter] = I2C_read_byte();

		/*Check if the next character stored if null
		 * if so, modify the counter to finish the loop*/
		if(NULL == mem_data[counter])
			counter = PAGE_SIZE;

		/*Increments to the next address*/
		address++;
		/*Increments to the next ptr of the array*/
		counter++;

	}while(counter < PAGE_SIZE);

	return mem_data;
}

void EEPROM_clear_mem()
{
	uint16 mem_size;
	do
	{
		mem_data[mem_size] = NULL;
		mem_size++;
	}while(mem_size < PAGE_SIZE);
}

void EEPROM_delay(uint32 delay){
	uint32 i=delay;
	while(i!=0){
		i--;
	}
}
