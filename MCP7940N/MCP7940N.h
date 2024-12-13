/**
	\file 	  MCP7940N.h
	\brief
			  This is the header file for the MCP7940N module. It contains functions
			  for the operation of the MCP7940N integrated circuit, a real time clock.
	\authors: César Villarreal Hernández, ie707560
	          José Luis Rodríguez Gutierrez,ie705694
	\date	  25/04/2019
 */


#ifndef MCP7940N_H_
#define MCP7940N_H_

#include "gpio.h"
#include "i2c.h"
#include "delay.h"
#include "bits.h"

#define MCP7940N_WRITE_CONTROL 0XDE
#define MCP7940N_READ_CONTROL 0XDF

#define ENABLE_ADDR 0X00
#define ENABLE_COUNT 0x80

#define SEC_ADDR  0x00
#define MIN_ADDR  0x01
#define HOUR_ADDR 0x02

#define DATE_ADDR 0x04
#define MONTH_ADDR 0x05
#define YEAR_ADDR 0x06

#define MCP7940N_DELAY 1000

#define MCP7940N_S0 0  //WRITE CONTROL STATE
#define MCP7940N_S1 1  //SET ADDRESS STATE
#define MCP7940N_S2 2  //WRITE DATA STATE
#define MCP7940N_S3 3
#define MCP7940N_S4 4  //WRITE CONTROL STATE
#define MCP7940N_S5 5  //SET ADDRESS STATE
#define MCP7940N_S6 6  //WRITE DATA STATE
#define MCP7940N_S7 7
#define MCP7940N_S8 8  //WRITE CONTROL STATE

typedef struct
{
	uint8_t Transmit_Receive;
	uint8_t writebyte;
	void(*fptrTxRx)(uint8_t tx_or_rx);
	void (*fptrWrite)(uint8_t data);
	void (*fptrWaitTransfer)(void);
	void (*fptrgetAck)(void);
	void (*start)(void);
	void (*stop)(void);
	uint8_t next[3];
}MCP7940N_state_t;

typedef struct
{
	uint8_t Transmit_Receive;
	uint8_t *writebyte;
	void(*fptrTxRx)(uint8_t tx_or_rx);
	void (*fptrWrite)(uint8_t data);
	void (*fptrWaitTransfer)(void);
	void (*fptrgetAck)(void);
	void (*start)(void);
	void (*stop)(void);
	uint8_t next[3];
}MCP7940N_time_state_t;
/*!
 	 \brief	 Funct to set a value to a register
 	 \param[in] void
 	 \return void
 */
void MCP7940N_enable(void);

/*!
 	 \brief	 Funct to set a value to a register
 	 \param[in] receive data to write in the register
 	 \return void
 */
void MCP7940N_set_sec(uint8_t data);

/*!
 	 \brief	 Funct to get the value from a register
 	 \param[in] void
 	 \return Value stored in a register
 */
uint8_t MCP7940N_get_sec(void);

/*!
 	 \brief	 Funct to set a value to a register
 	 \param[in] receive data to write in the register
 	 \return void
 */
void MCP7940N_set_min(uint8_t data);

/*!
 	 \brief	 Funct to get the value from a register
 	 \param[in] void
 	 \return Value stored in a register
 */
uint8_t MCP7940N_get_min(void);

/*!
 	 \brief	 Funct to set a value to a register
 	 \param[in] receive data to write in the register
 	 \return void
 */
void MCP7940N_set_hour(uint8_t data);

/*!
 	 \brief	 Funct to get the value from a register
 	 \param[in] void
 	 \return Value stored in a register
 */
uint8_t MCP7940N_get_hour(void);

/*!
 	 \brief	 Funct to set a value to a register
 	 \param[in] receive data to write in the register
 	 \return void
 */
void MCP7940N_set_date(uint8_t data);

/*!
 	 \brief	 Funct to get the value from a register
 	 \param[in] void
 	 \return Value stored in a register
 */
uint8_t MCP7940N_get_date(void);

/*!
 	 \brief	 Funct to set a value to a register
 	 \param[in] receive data to write in the register
 	 \return void
 */
void MCP7940N_set_month(uint8_t data);

/*!
 	 \brief	 Funct to get the value from a register
 	 \param[in] void
 	 \return Value stored in a register
 */
uint8_t MCP7940N_get_month(void);

/*!
 	 \brief	 Funct to set a value to a register
 	 \param[in] receive data to write in the register
 	 \return void
 */
void MCP7940N_set_year(uint8_t data);

/*!
 	 \brief	 Funct to get the value from a register
 	 \param[in] void
 	 \return Value stored in a register
 */
uint8_t MCP7940N_get_year(void);

/*!
 	 \brief	 Funct to delay the process so it can work properly.
 	 \param[in] delay
 	 \return void
 */
void MCP7940N_delay(uint32_t delay);

#endif /* MCP7940N_H_ */
