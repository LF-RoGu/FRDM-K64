/*
 * SPI.c
 *
 *  Created on: Oct 2, 2018
 *      Author: LuisFernando
 */

#include "SPI.h"

/*
 * MCR; Module Configuration Register*/

/*it enable the clock module of the SPI by modifying the MDIS bits*/
static void SPI_enable(spi_channel_t channel)
{
	switch (channel)
	{
	case SPI_0:
		/*Buscar la mascara que habilite el MCR del SPI en el modulo MK64F12*/
		SPI0->MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	case SPI_1:
		/*Buscar la mascara que habilite el MCR del SPI en el modulo MK64F12*/
		SPI1->MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	case SPI_2:
		/*Buscar la mascara que habilite el MCR del SPI en el modulo MK64F12*/
		SPI2->MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	default:
		break;
	}
}
/*It activate the clock gating*/
static void SPI_clk(spi_channel_t channel)
{
	switch (channel)
	{
	case SPI_0:
		/*Buscar la mascara que habilite el MCR del SPI en el modulo MK64F12*/
		SIM->SCGC6 |= SIM_SCGC6_SPI0_MASK;
		break;
	case SPI_1:
		/*Buscar la mascara que habilite el MCR del SPI en el modulo MK64F12*/
		SIM->SCGC6 |= SIM_SCGC6_SPI1_MASK;
		break;
	case SPI_2:
		/*Buscar la mascara que habilite el MCR del SPI en el modulo MK64F12*/
		SIM->SCGC3 |= SIM_SCGC3_SPI2_MASK;
		break;
	default:
		break;
	}
}
/*It configure the SPI as a master or slave depending on the value of masterOrslave*/
static void SPI_set_master(spi_channel_t channel, spi_master_t masterOrSlave)
{
	switch (channel)
	{
	case SPI_0:
		if(SPI_SLAVE == masterOrSlave)
			SPI0->MCR &= ~(SPI_MCR_MSTR_MASK);
		else if(SPI_MASTER == masterOrSlave)
			SPI0->MCR |= (SPI_MCR_MSTR_MASK);
		break;
	case SPI_1:
		if (SPI_SLAVE == masterOrSlave)
			SPI1->MCR &= ~(SPI_MCR_MSTR_MASK);
		else if(SPI_MASTER == masterOrSlave)
			SPI1->MCR |= (SPI_MCR_MSTR_MASK);
		break;
	case SPI_2:
		if (SPI_SLAVE == masterOrSlave)
			SPI2->MCR &= ~(SPI_MCR_MSTR_MASK);
		else if(SPI_MASTER == masterOrSlave)
			SPI2->MCR |= (SPI_MCR_MSTR_MASK);
		break;
	default:
		break;
	}
}
/*It activate the TX and RX FIFOs of the SPI depending on the value of enableOrdisable*/
static void SPI_fifo(spi_channel_t channel, spi_enable_fifo_t enableOrDisable)
{
	switch (channel)
	{
	case SPI_0:
		if(SPI_DISABLE_FIFO == enableOrDisable)
		{
			SPI0->MCR |= (SPI_MCR_DIS_RXF_MASK);
			SPI0->MCR |= (SPI_MCR_DIS_TXF_MASK);
		}
		else if(SPI_ENABLE_FIFO == enableOrDisable)
		{
			SPI0->MCR &= ~(SPI_MCR_DIS_RXF_MASK);
			SPI0->MCR &= ~(SPI_MCR_DIS_TXF_MASK);
		}
		break;
	case SPI_1:
		if (SPI_DISABLE_FIFO == enableOrDisable) {
			SPI1->MCR |= (SPI_MCR_DIS_RXF_MASK);
			SPI1->MCR |= (SPI_MCR_DIS_TXF_MASK);
		} else if (SPI_ENABLE_FIFO == enableOrDisable) {
			SPI1->MCR &= ~(SPI_MCR_DIS_RXF_MASK);
			SPI1->MCR &= ~(SPI_MCR_DIS_TXF_MASK);
		}
		break;
	case SPI_2:
		if (SPI_DISABLE_FIFO == enableOrDisable) {
			SPI2->MCR |= (SPI_MCR_DIS_RXF_MASK);
			SPI2->MCR |= (SPI_MCR_DIS_TXF_MASK);
		} else if (SPI_ENABLE_FIFO == enableOrDisable) {
			SPI2->MCR &= ~(SPI_MCR_DIS_RXF_MASK);
			SPI2->MCR &= ~(SPI_MCR_DIS_TXF_MASK);
		}
		break;
	default:
		break;
	}
}
/*It selects the clock polarity depending on the value of cpol*/
static void SPI_clock_polarity(spi_channel_t channel, spi_polarity_t cpol)
{
	switch(channel)
	{
	case SPI_0:
		if (SPI_LOW_POLARITY == cpol)
			SPI0->CTAR[0] &= ~(SPI_CTAR_CPOL_MASK);
		else if (SPI_HIGH_POLARITY == cpol)
			SPI0->CTAR[0] |= (SPI_CTAR_CPOL_MASK);
		break;
	case SPI_1:
		if (SPI_LOW_POLARITY == cpol)
			SPI1->CTAR[0] &= ~(SPI_CTAR_CPOL_MASK);
		else if (SPI_HIGH_POLARITY == cpol)
			SPI1->CTAR[0] |= (SPI_CTAR_CPOL_MASK);
		break;
	case SPI_2:
		if (SPI_LOW_POLARITY == cpol)
			SPI2->CTAR[0] &= ~(SPI_CTAR_CPOL_MASK);
		else if (SPI_HIGH_POLARITY == cpol)
			SPI2->CTAR[0] |= (SPI_CTAR_CPOL_MASK);
		break;
	default:
		break;

	}
}
/*It selects the frame size depending on the value of frameSize and the macros that are defined above*/
static void SPI_frame_size(spi_channel_t channel, uint32_t frameSize)
{
	/*FMSZ 16bits*/
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[0] &= ~(SPI_CTAR_FMSZ_MASK);
		SPI0->CTAR[0] |= frameSize;
		break;
	case SPI_1:
		SPI1->CTAR[0] &= ~(SPI_CTAR_FMSZ_MASK);
		SPI1->CTAR[0] |= frameSize;
		break;
	case SPI_2:
		SPI2->CTAR[0] &= ~(SPI_CTAR_FMSZ_MASK);
		SPI2->CTAR[0] |= frameSize;
		break;
	default:
		break;
	}
}
/*It selects the clock phase depending on the value of cpha*/
static void SPI_clock_phase(spi_channel_t channel, spi_phase_t cpha)
{
	switch (channel)
	{
		case SPI_0:
			if (SPI_LOW_PHASE == cpha)
				SPI0->CTAR[0] &= ~(SPI_CTAR_CPHA_MASK);
			else if (SPI_HIGH_PHASE == cpha)
				SPI0->CTAR[0] |= (SPI_CTAR_CPHA_MASK);
			break;
		case SPI_1:
			if (SPI_LOW_PHASE == cpha)
				SPI1->CTAR[0] &= ~(SPI_CTAR_CPHA_MASK);
			else if (SPI_HIGH_PHASE == cpha)
				SPI1->CTAR[0] |= (SPI_CTAR_CPHA_MASK);
			break;
		case SPI_2:
			if (SPI_LOW_PHASE == cpha)
				SPI2->CTAR[0] &= ~(SPI_CTAR_CPHA_MASK);
			else if (SPI_HIGH_PHASE == cpha)
				SPI2->CTAR[0] |= (SPI_CTAR_CPHA_MASK);
			break;
		default:
			break;
	}
}
/*It selects the baud rate depending on the value of baudRate and the macros that are defined above*/
static void SPI_baud_rate(spi_channel_t channel, uint32_t baudRate)
{
	switch(channel)
	{
			case SPI_0:
				SPI0->CTAR[0] &= ~(SPI_CTAR_BR_MASK);
				SPI0->CTAR[0] |= baudRate;
				break;
			case SPI_1:
				SPI1->CTAR[0] &= ~(SPI_CTAR_BR_MASK);
				SPI1->CTAR[0] |= baudRate;
				break;
			case SPI_2:
				SPI2->CTAR[0] &= ~(SPI_CTAR_BR_MASK);
				SPI2->CTAR[0] |= baudRate;
				break;
	}
}
/*It selects if MSB or LSM bits is first transmitted*/
static void SPI_msb_first(spi_channel_t channel, spi_lsb_or_msb_t msb)
{
	switch(channel)
	{
	case SPI_0:
		if(SPI_MSB == msb)
			SPI0->CTAR[0] &= ~(SPI_CTAR_LSBFE_MASK);
		else if(SPI_LSM == msb)
			SPI0->CTAR[0] |= (SPI_CTAR_LSBFE_MASK);
		break;
	case SPI_1:
			if(SPI_MSB == msb)
				SPI1->CTAR[0] &= ~(SPI_CTAR_LSBFE_MASK);
			else if(SPI_LSM == msb)
				SPI1->CTAR[0] |= (SPI_CTAR_LSBFE_MASK);
			break;
	case SPI_2:
			if(SPI_MSB == msb)
				SPI2->CTAR[0] &= ~(SPI_CTAR_LSBFE_MASK);
			else if(SPI_LSM == msb)
				SPI2->CTAR[0] |= (SPI_CTAR_LSBFE_MASK);
			break;
	}
}
/*It stars the SPI transmission by modifying the value of HALT bit*/
void SPI_start_tranference(spi_channel_t channel)
{
	switch(channel)
	{
		case SPI_0:
			SPI0->MCR &= ~(SPI_MCR_HALT_MASK);
			break;
		case SPI_1:
			SPI1->MCR &= ~(SPI_MCR_HALT_MASK);
			break;
		case SPI_2:
			SPI2->MCR &= ~(SPI_MCR_HALT_MASK);
			break;
	}
}
/*It stops the SPI transmission by modifying the value of HALT bit*/
void SPI_stop_tranference(spi_channel_t channel)
{
	switch (channel) {
	case SPI_0:
		SPI0->MCR |= (SPI_MCR_HALT_MASK);
		break;
	case SPI_1:
		SPI1->MCR |= (SPI_MCR_HALT_MASK);
		break;
	case SPI_2:
		SPI2->MCR |= (SPI_MCR_HALT_MASK);
		break;
	}
}
/*It transmits the information contained in data*/
void SPI_send_one_byte(uint8_t Data)
{
	SPI0->PUSHR = Data;
	while(0 == (SPI0->SR & SPI_SR_TCF_MASK));
	SPI0->SR |= SPI_SR_TCF_MASK;
}
/*It configures the SPI for transmission, this function as arguments receives a pointer to a constant structure where are all
 * the configuration parameters*/
void SPI_init(const spi_config_t* SPI_Config)
{
	/*Config inicial no funciono, asi que pregunte a un compañero y me recomendo re-ajustar a primero los clk, luego la config M/S*/
	SPI_clk(SPI_Config->spi_channel);
	/**/
	GPIO_clock_gating(SPI_Config->spi_gpio_port.gpio_port_name);
	GPIO_pin_control_register(SPI_Config->spi_gpio_port.gpio_port_name,SPI_Config->spi_gpio_port.spi_clk,&(SPI_Config->pin_config));
	GPIO_pin_control_register(SPI_Config->spi_gpio_port.gpio_port_name,SPI_Config->spi_gpio_port.spi_sout,&(SPI_Config->pin_config));
	/**/
	SPI_set_master(SPI_Config->spi_channel,SPI_Config->spi_master);
	SPI_fifo(SPI_Config->spi_channel,SPI_Config->spi_enable_fifo);
	SPI_enable(SPI_Config->spi_channel);

	SPI_clock_polarity(SPI_Config->spi_channel,SPI_Config->spi_polarity);
	SPI_frame_size(SPI_Config->spi_channel,SPI_Config->spi_frame_size);
	SPI_clock_phase(SPI_Config->spi_channel,SPI_Config->spi_phase);
	SPI_baud_rate(SPI_Config->spi_channel,SPI_Config->spi_baudrate);
	SPI_msb_first(SPI_Config->spi_channel,SPI_Config->spi_lsb_or_msb);
}
