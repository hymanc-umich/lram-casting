/*
 * clock_setup.c
 * Basic ATxmega clock system setup for AirSSI
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 

#include "clock_setup.h"

/* 
 * @brief Initializes the main clock for 4MHz operation based on a
 * 2x PLL multiplication of the 2MHz RC Oscillator
 */
void clock_initialize(void)
{
	OSC.PLLCTRL = OSC_PLLSRC_RC2M_gc | OSC_PLLFAC2_bm; // Set PLL x2 for 4MHz
	OSC.CTRL |= OSC_PLLEN_bm;	// Enable PLL
	while(!(OSC.STATUS & OSC_PLLRDY_bm))
	{
		__asm__("NOP");			// Wait for PLL to be ready
	}
	CCP = CCP_IOREG_gc;			// Enable CCP write
	CLK.CTRL = CLK_SCLKSEL_PLL_gc; // Select PLL clock as source in CCP write
}