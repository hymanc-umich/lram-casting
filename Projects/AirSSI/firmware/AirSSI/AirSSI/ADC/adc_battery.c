/*
 * adc_battery.c
 * Battery monitor ADC driver
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 

#include "adc_battery.h"
#include <stddef.h>
#include <avr/pgmspace.h>

/*
 *
 *
 */
void adc_initialize(void)
{ 
	BATTERY_ADC.REFCTRL = ADC_REFSEL_VCC_gc;
	BATTERY_ADC.PRESCALER = ADC_PRESCALER_DIV8_gc;
	//BATTERY_ADC.SAMPCTRL = 7;
	BATTERY_ADC.CAL = 0; // GET SIG BYTE
	BATTERY_ADC.CTRLA |= ADC_ENABLE_bm;
	adc_calibrate();
}

/*
 *
 *
 */
void adc_start_read(uint8_t adc_addr)
{
	BATTERY_ADC.CH0.MUXCTRL = adc_addr << 3;
	BATTERY_ADC.CH0.CTRL |= ADC_CH0START_bm;
}

/*
 *
 *
 */
uint8_t adc_has_data(void)
{
	if(BATTERY_ADC.CH0.INTFLAGS & ADC_CH0IF_bm)
	{
		return 1;
	}
	return 0;
}

/*
 *
 *
 */
uint16_t adc_get_data(void)
{
	if(adc_has_data)
	{
		return BATTERY_ADC.CH0RES;
	}
	else return 0xFFFF; // Error value
}

/*
 *
 *
 */
uint8_t adc_read_cal_signature(uint8_t index)
{
	uint8_t ret_val;
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	ret_val = pgm_read_byte(index);
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;
	return ret_val;
}

/*
 *
 *
 */
void adc_calibrate(void)
{
	BATTERY_ADC.CALL = adc_read_cal_signature( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
	BATTERY_ADC.CALH = adc_read_cal_signature( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));
}