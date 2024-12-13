/*
 * DAC.h
 *
 *  Created on: Sep 24, 2018
 *      Author: LuisFernando
 */

#ifndef DAC_H_
#define DAC_H_

#include "MK64F12.h"
#include "GPIO.h"
#include "DataTypeDefinitions.h"
#include "PIT.h"



void DAC_clock_gating(void);

void DAC_enable(BooleanType enable);

void DAC_set_value_sine(void);

void DAC_set_value_tri(void);

void DAC_set_value_square(void);

#endif /* DAC_H_ */
