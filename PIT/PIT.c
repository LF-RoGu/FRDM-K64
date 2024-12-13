/*
 * PIT.c
 *
 *  Created on: Sep 11, 2018
 *      Author: LuisFernando
 *      PIT_IRQHandler baasado en Manuel Alejandro obtenido de la pagina de NXP
 */

#include "MK64F12.h"
#include "PIT.h"
#include "Bits.h"

static uint8 intrFlag0 = FALSE;
static uint8 intrFlag1 = FALSE;
static uint8 intrFlag2 = FALSE;
static uint8 intrFlag3 = FALSE;

void PIT_delay(PIT_Timer_t pitTimer,ufloat32 systemClock ,ufloat32 period)
{
	/**Es necesario hacer un cast para pasar del numero float a entero, y de ahi a hexa**/
	ufloat32 clockPeriod = 0.0F;
	uint32 cycles = 0;
	systemClock = systemClock/2;
	clockPeriod = 1/systemClock;
	cycles = (uint32)(period/clockPeriod);
	cycles = cycles - 1;

	PIT->CHANNEL[pitTimer].LDVAL = cycles;
	PIT_enable_interrupt(pitTimer);
}

void PIT_enable(void)
{
	PIT->MCR = MCR_ON;
}

void PIT_stop(void)
{
	PIT->MCR = MCR_OFF;
}

void PIT_enable_interrupt(PIT_Timer_t pitTimer)
{
	/**
	 * TIE enables interrupts for the timer
	 * TEN starts the timer**/
	PIT->CHANNEL[pitTimer].TCTRL |= PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
}

void PIT_clock_gating(void)
{
	/**
	 * Enable PIT clock
	 * **/
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}
/*
 * Funct called to get intr status*/
uint8 PIT0_get_interrupt_status(void)
{
	return intrFlag0;
}
uint8 PIT1_get_interrupt_status(void)
{
	return intrFlag1;
}
uint8 PIT2_get_interrupt_status(void)
{
	return intrFlag2;
}
uint8 PIT3_get_interrupt_status(void)
{
	return intrFlag3;
}
/*
 * Funct called to clear all PIT's intrFlags*/
void PIT_clear(void)
{
	intrFlag0 = TRUE;
	intrFlag1 = TRUE;
	intrFlag2 = TRUE;
	intrFlag3 = TRUE;
}
/**
 * Funct called from startup_mk64f12.c**/
/*
 * Funct call to deal with PIT's IRQ*/
void PIT0_IRQHandler(void)
{
	PIT->CHANNEL[PIT_0].TFLG |= PIT_TFLG_TIF_MASK;
	/**
	 * Read control reg for clear PIT flag
	 * **/
	PIT->CHANNEL[PIT_0].TCTRL;
	/**
	 * Enables PIT timer interrupt
	 * **/
	PIT->CHANNEL[PIT_0].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	/**Enables timer cero**/
	PIT->CHANNEL[PIT_0].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	/****/
	intrFlag0 = TRUE;
}
void PIT1_IRQHandler(void)
{
	PIT->CHANNEL[PIT_1].TFLG |= PIT_TFLG_TIF_MASK;
	/**
	 * Read control reg for clear PIT flag
	 * **/
	PIT->CHANNEL[PIT_1].TCTRL;
	/**
	 * Enables PIT timer interrupt
	 * **/
	PIT->CHANNEL[PIT_1].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	/**Enables timer cero**/
	PIT->CHANNEL[PIT_1].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	/****/
	intrFlag1 = TRUE;
}
void PIT2_IRQHandler(void)
{
	PIT->CHANNEL[PIT_2].TFLG |= PIT_TFLG_TIF_MASK;
	/**
	 * Read control reg for clear PIT flag
	 * **/
	PIT->CHANNEL[PIT_2].TCTRL;
	/**
	 * Enables PIT timer interrupt
	 * **/
	PIT->CHANNEL[PIT_2].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	/**Enables timer cero**/
	PIT->CHANNEL[PIT_2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	/****/
	intrFlag2 = TRUE;
}
void PIT3_IRQHandler(void)
{
	PIT->CHANNEL[PIT_3].TFLG |= PIT_TFLG_TIF_MASK;
	/**
	 * Read control reg for clear PIT flag
	 * **/
	PIT->CHANNEL[PIT_3].TCTRL;
	/**
	 * Enables PIT timer interrupt
	 * **/
	PIT->CHANNEL[PIT_3].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	/**Enables timer cero**/
	PIT->CHANNEL[PIT_3].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	/****/
	intrFlag3 = TRUE;
}
