/*
 * PIT.h
 *
 *  Created on: Sep 11, 2018
 *      Author: LuisFernando
 */

#ifndef PIT_H_
#define PIT_H_

#include "Bits.h"
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function configure the PIT to generate a delay base on the system clock.
 	 Internally it configures the clock gating and enables the PIT module.
 	 It is important to note that this strictly is not device driver since everything is
 	 contained in a single function, but in general you have to avoid this practices, this only
 	 for the propose of the homework

 	 \param[in]  portName Port to be configured.
 	 \return void
 */

typedef enum {PIT_0,PIT_1,PIT_2,PIT_3}PIT_Timer_t;
typedef enum {MCR_ON,MCR_OFF}MCR_State;
void PIT_delay(PIT_Timer_t pitTimer,ufloat32 systemClock ,ufloat32 period);

void PIT_clock_gating(void);

void PIT_enable(void);

void PIT_stop(void);

void PIT_LDVAL(PIT_Timer_t pitTimer);

void PIT_enable_interrupt(PIT_Timer_t pitTimer);

uint8 PIT0_get_interrupt_status(void);
uint8 PIT1_get_interrupt_status(void);
uint8 PIT2_get_interrupt_status(void);
uint8 PIT3_get_interrupt_status(void);

void PIT_clear(void);

uint32 decToHexa(uint32 value);

void PIT0_IRQHandler(void);
void PIT1_IRQHandler(void);
void PIT2_IRQHandler(void);
void PIT3_IRQHandler(void);

#endif /* PIT_H_ */
