/*  ssi.c A bit-bang Synchronous Serial Interface Driver for STM32
    Copyright (C) 2013 Cody Hyman, OSU Lab for Robotics & Applied Mechanics

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ch.h"
#include "hal.h"

#include "ssi.h"
#include "bluetooth.h"
#include "pwr_control.h"

volatile uint32_t ssi_read_delay = 2000; // Default to 2ms delay
volatile uint8_t ssi_mode = SSI_MODE_POLL;	// SSI Mode 
volatile uint8_t encoder_power_state = ENCODER_OFF; // Encoder Power State
volatile uint8_t led_count = 0; // LED Counter
/**
 * @brief Internal thread initialization function that sets up SSI I/O and data structures
 */
static void ssi_init(void)
{
	// Initialize timer and I/O
	palSetPadMode(SSI_SIGNAL_PORT, SSI_CLK_PIN, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPad(SSI_SIGNAL_PORT, SSI_CLK_PIN);
	palSetPadMode(SSI_SIGNAL_PORT, SSI_DATA_PIN, PAL_MODE_INPUT);
	palSetPadMode(SSI_CTL_PORT, SSI_TXEN_PIN, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(SSI_CTL_PORT, SSI_NRXEN_PIN, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(SSI_CTL_PORT, SSI_PWREN_PIN, PAL_MODE_OUTPUT_PUSHPULL);
	SSI_TXEN_ON();	// Turn on RS-422 transmitter
	SSI_RXEN_ON(); 	// Turn on RS-422 receiver
	ssi_encoder_power(ENCODER_ON); // Turn on encoder power
}

/**
 * @brief Function to turn encoder power on and off
 * @param uint8_t pwr_state The power state to set the encoder to
 */
void ssi_encoder_power(uint8_t pwr_state)
{
	if(pwr_state == ENCODER_ON)	// Turn encoder system on
	{
		encoder_power_state = ENCODER_ON;
		palSetPad(SSI_CTL_PORT, SSI_PWREN_PIN);
		SSI_TXEN_ON();
		SSI_RXEN_ON();
	}
	else						// Turn encoder system off
	{
		encoder_power_state = ENCODER_OFF;
		palClearPad(SSI_CTL_PORT, SSI_PWREN_PIN);
		SSI_TXEN_OFF();
		SSI_RXEN_OFF();
	}
}

/**
 * @brief Function to increase the SSI stream delay by 50us to 100ms
 */
void ssi_increase_delay(void)
{
	if(ssi_read_delay < 100000)
	{
		ssi_read_delay += 50;
	}
}

/**
 * @breif Function to decrease the SSI stream delay by 50us to 1ms
 */
void ssi_decrease_delay(void)
{
	if(ssi_read_delay > 1000)
	{
		ssi_read_delay -= 50;
	}
}
/**
 * @brief SSI filler fixed delay for SW clock timing
 */
static inline void ssi_delay(void)
{
	uint16_t del_ctr;
	for(del_ctr = 0; del_ctr < 3; del_ctr++)
	{
		__asm__("NOP");
	}
}

/**
 * @brief Sets the SSI mode between a continuous stream and polling mode
 * @param uint8_t mode The SSI mode to use, polled or streaming
 */
void ssi_set_mode(uint8_t mode)
{
	if(mode == SSI_MODE_STREAM)
		ssi_mode = SSI_MODE_STREAM;
	else
	{
		ssi_mode = SSI_MODE_POLL;
	}
}

/**
 * @brief Performs a single SSI read in poll mode
 * @return bool Binary success of sending result data to the bluetooth module
 */
bool ssi_poll(void)
{
	uint32_t data = ssi_read();
	return bt_send_position(data);
}

/**
 * @brief Single word read from the SSI encoder
 * @return Most recent SSI byte (may also be buffered in stream mode)
 */
uint32_t ssi_read(void)
{
	uint8_t bit = SSI_LENGTH; // Set length counter
	uint32_t working_buffer = 0;
	if(ssi_mode == SSI_MODE_STREAM)
	{
		led_count = (led_count+1)%25;// LED handling
		if(!led_count)
			led_toggle();
	}
	while(bit > 0)
	{
		// Falling edge
		palClearPad(SSI_SIGNAL_PORT, SSI_CLK_PIN);	// Toggle clock
		ssi_delay();
		ssi_delay();
		ssi_delay();
		// Rising edge
		palSetPad(SSI_SIGNAL_PORT, SSI_CLK_PIN);	// Toggle clock
		working_buffer = (working_buffer << 1) + palReadPad(SSI_SIGNAL_PORT, SSI_DATA_PIN);
		ssi_delay();
		bit--;	// Decrement bit counter
	}
	working_buffer = ((1<<SSI_LENGTH) - 1) & (~working_buffer); // Invert bits in buffer to simulate 
	return working_buffer;
}

/**
 * @brief Mutator function for stream mode read-to-read delay setting
 * @param uint16_t delay_us The new delay in microseconds
 */
void ssi_set_delay(uint16_t delay_us)
{
	ssi_read_delay = delay_us;
}

/**
 * @brief SSI thread main function
 * @param void * arg Thread arguments (NULL)
 * @return msg_t Thread messages
 */
msg_t ssi_main(void *arg)
{
	ssi_init();			// Initialize SSI Encoder
	uint32_t position;	// Position buffer
	
	// Thread main loop
	systime_t time = chTimeNow();
	while(1)
	{
		time = chTimeNow();
		if((ssi_mode == SSI_MODE_STREAM) && (encoder_power_state == ENCODER_ON))
		{
			time += US2ST(ssi_read_delay);
			position = ssi_read();	// Read SSI Encoder
			bt_send_position(position); // Send out encoder data via serial
			while(!bt_get_tx_state())
			{
				__asm__("NOP"); // Wait for position send to complete 
			}
			chThdSleepUntil(time);
		}
		else
		{
			chThdSleepMilliseconds(500);
		}
	}
}
