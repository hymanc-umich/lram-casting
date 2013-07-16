/*
 * rm_encoder.c
 * RLS Magnetic SSI Encoder Interface for the ATXmega D4 
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 
#include "rm_encoder.h"

volatile uint8_t encoder_power_state;
volatile uint16_t encoder_value;
volatile uint8_t encoder_value_valid_flag;

/*
 * @brief Initializes the encoder SSI and powers it off
 */
void rm_encoder_initialize(void)
{
	// Initialize Encoder Power Switch to Off (0 out)
	ENC_EN_PORT.OUTCLR = ENC_EN_bm;
	ENC_EN_PORT.DIRSET = ENC_EN_bm;
	encoder_power_state = ENCODER_OFF;
	
	encoder_value_valid_flag = FALSE;	// Set default encoder value valid flag to false
	
	// Initialize SSI Driver
	ssi_initialize();
	ssi_set_enc_handler(&rm_encoder_hanlder);
}

/* 
 * @brief Requests and returns the encoder position in a blocking fashion 
 */
uint16_t rm_encoder_read_blocking(void)
{
	if(encoder_power_state == ENCODER_ON)
	{
		// Wait for previous read to finish
		while(ssi_read_in_progress())
		{
			__asm__("NOP"); // NOP to prevent optimization
		}
		if(ssi_start_read()) // Check for successful read start
		{
			while(ssi_read_in_progress())
			{
				__asm__("NOP");
			}
			// Check and return data
		}
		else
			return 0xFFFF;
	}
	else
		return 0xFFFF; // Return ERROR
}

/* 
 * @brief Turns on the encoder power 
 */
void rm_encoder_power_on(void)
{
	ENC_EN_PORT.OUTSET = ENC_EN_bm;
	encoder_power_state = ENCODER_ON;
}

/* 
 * @brief Turns off the encoder power 
 */
void rm_encoder_power_off(void)
{
	ENC_EN_PORT.OUTCLR = ENC_EN_bm;
	encoder_power_state = ENCODER_OFF;
}

/* 
 * @brief Resets the encoder via a power cycle with a ~100ms delay
 */
void rm_encoder_reset(void)
{
	rm_encoder_power_off();
	_delay_ms(ENCODER_RESET_DELAY);
	rm_encoder_power_on();
}

/* 
 * @brief Returns the power state (on/off) of the encoder 
 */
uint8_t rm_encoder_get_power_state(void)
{
	return encoder_power_state;
}

/* 
 * @brief Asynchronous high-level handler for SSI encoder data storage
 */
uint16_t rm_encoder_hanlder(uint16_t readout_value)
{
	encoder_value = readout_value;
	encoder_value_valid_flag = TRUE;
	return readout_value;
}