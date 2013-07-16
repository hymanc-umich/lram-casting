/*
 * AirSSI.c
 * Firmware for the LRAM AirSSI wireless encoder module
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 

#define F_CPU 4000000UL // 4MHz Operating Frequency

#include <avr/io.h>
#include <util/delay.h>

#include "ADC/adc_battery.h"
#include "Encoder/rm_encoder.h"
#include "Radio/radio.h"
#include "Util/clock_setup.h"
#include "Radio/cc1101/SPI/spi_master.h"
#include "Radio/cc1101/cc1101.h"

int main(void)
{
    clock_initialize();
	adc_initialize();
	rm_encoder_initialize();
	rm_encoder_power_on();
	cc1101_initialize();
	
	
	while(1)
    {
		// Periodically update encoder value
		rm_encoder_read_blocking();
		_delay_us(250);
		//spi_master_transact(0xA9);
    }
	return 0; // End return state
}