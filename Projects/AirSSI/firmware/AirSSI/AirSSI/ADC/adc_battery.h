/*
 * adc_battery.h
 * Battery monitor ADC driver
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 


#ifndef ADC_BATTERY_H_
#define ADC_BATTERY_H_

#include <avr/io.h>

// Fix these battery thresholds
#define BATTERY_LOW_THRESHOLD 100
#define BATTERY_CRITICAL_THRESHOLD 100

// ADC location defines
#define BATTERY_ADC ADCA
#define BATTERY_ADC_CH 2


void adc_initialize(void);
void adc_start_read(uint8_t adc_addr);
uint8_t adc_has_data(void);
uint16_t adc_get_data(void);


#endif /* ADC_BATTERY_H_ */