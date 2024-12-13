/*
 * DAC.c
 *
 *  Created on: Sep 21, 2018
 *      Author: LuisFernando
 */
/*
 * Para pasar a 12 bits lo que se hace es una regla de 3
 * en cuanto a uno de 16 bits y uno de 12 bits y se multiplica por el valor*/

#include "DAC.h"
#include "RGB.h"
#include "DataTypeDefinitions.h"
#include "PIT.h"

/*size of the array*/
#define SIZEWAVE 101
/*PIT time*/
#define SYSTEM_CLOCK 21000000
#define DELAY 0.00396F //5Hz

uint16 sineWave[SIZEWAVE] = { 2047, 2176, 2304, 2431, 2556, 2680, 2801, 2919,
		3033, 3144, 3250, 3352, 3448, 3539, 3624, 3703, 3775, 3841, 3899, 3950,
		3994, 4030, 4058, 4078, 4090, 4094, 4090, 4078, 4058, 4030, 3994, 3950,
		3899, 3841, 3775, 3703, 3624, 3539, 3448, 3352, 3250, 3144, 3033, 2919,
		2801, 2680, 2556, 2431, 2304, 2176, 2047, 1918, 1790, 1663, 1538, 1414,
		1293, 1175, 1061, 950, 844, 742, 646, 555, 470, 391, 319, 253, 195, 144,
		100, 64, 36, 16, 4, 0, 4, 16, 36, 64, 100, 144, 195, 253, 319, 391, 470,
		555, 646, 742, 844, 950, 1061, 1175, 1293, 1414, 1538, 1663, 1790, 1918,
		2047 };
uint16 triangularWave[SIZEWAVE] = { 0, 82, 164, 246, 328, 409, 491, 573, 655,
		737, 819, 901, 983, 1064, 1146, 1228, 1310, 1392, 1474, 1556, 1638,
		1719, 1801, 1883, 1965, 2047, 2129, 2211, 2293, 2375, 2456, 2538, 2620,
		2702, 2784, 2866, 2948, 3030, 3111, 3193, 3275, 3357, 3439, 3521, 3603,
		3685, 3766, 3848, 3930, 4012, 4094, 4012, 3930, 3848, 3766, 3685, 3603,
		3521, 3439, 3357, 3275, 3193, 3111, 3030, 2948, 2866, 2784, 2702, 2620,
		2538, 2456, 2375, 2293, 2211, 2129, 2047, 1965, 1883, 1801, 1719, 1638,
		1556, 1474, 1392, 1310, 1228, 1146, 1064, 983, 901, 819, 737, 655, 573,
		491, 409, 328, 246, 164, 82, 0 };
uint16 squareWave[SIZEWAVE] = { 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4094 };

void DAC_clock_gating(void) {
	SIM->SCGC2 |= SIM_SCGC2_DAC0_MASK;
	DAC0->C0 = FALSE;
	DAC0->C1 = FALSE;
	DAC0->C0 = (DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK);
}

void DAC_set_value_sine(void) {
	uint8 i = 0;
	uint8 pitIntrStatus = FALSE;
	/*clear leds*/
	RGB_off();
	/*RGB ON*/
	RGB_red_led_on_off(LED_ON);
	/*External LED's with PIT*/
	GPIO_toogle_pin(GPIO_D, BIT0);
	GPIO_clear_pin(GPIO_D, BIT2);

	for (i = 0; i < SIZEWAVE; i++) {
		/*8 bits en el registro DATL*/
		DAC0->DAT[0].DATL = ((0xFF & sineWave[i]));
		/*4 bits en el registro DATH*/
		DAC0->DAT[0].DATH = ((0xF00 & sineWave[i]) >> 8);
		PIT_delay(PIT_0, SYSTEM_CLOCK, DELAY);
		do {
			pitIntrStatus = PIT_get_interrupt_status();
		} while (FALSE == pitIntrStatus);
		PIT_clear();
	}
	/*RGB OFF*/
	RGB_red_led_on_off(LED_OFF);
}
void DAC_set_value_tri(void) {
	uint8 i = 0;
	uint8 pitIntrStatus = FALSE;
	/*clear leds*/
	RGB_off();
	/*RGB ON*/
	RGB_green_led_on_off(LED_ON);
	/*External LED's*/
	GPIO_clear_pin(GPIO_D, BIT0);
	GPIO_clear_pin(GPIO_D, BIT0);


	for (i = 0; i < SIZEWAVE; i++) {
		/*8 bits en el registro DATL*/
		DAC0->DAT[0].DATL = ((0xFF & triangularWave[i]));
		/*4 bits en el registro DATH*/
		DAC0->DAT[0].DATH = ((0xF00 & triangularWave[i]) >> 8);
		PIT_delay(PIT_0, SYSTEM_CLOCK, DELAY);
		do {
			pitIntrStatus = PIT_get_interrupt_status();
		} while (FALSE == pitIntrStatus);
		PIT_clear();
	}
	/*RGB OFF*/
	RGB_green_led_on_off(LED_OFF);
}
void DAC_set_value_square(void) {
	uint8 i = 0;
	uint8 pitIntrStatus = FALSE;
	/*clear leds*/
	RGB_off();
	/*RGB ON*/
	RGB_blue_led_on_off(LED_ON);
	/*External LED's*/
	GPIO_toogle_pin(GPIO_D, BIT2);
	GPIO_clear_pin(GPIO_D, BIT0);

	for (i = 0; i <= SIZEWAVE; i++) {
		/*8 bits en el registro DATL*/
		DAC0->DAT[0].DATL = ((0xFF & squareWave[i]));
		/*4 bits en el registro DATH*/
		DAC0->DAT[0].DATH = ((0xF00 & squareWave[i]) >> 8);
		PIT_delay(PIT_0, SYSTEM_CLOCK, DELAY);
		do {
			pitIntrStatus = PIT_get_interrupt_status();
		} while (FALSE == pitIntrStatus);
		PIT_clear();
	}
	/*RGB OFF*/
	RGB_blue_led_on_off(LED_OFF);
}
