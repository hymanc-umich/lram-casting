/*  ssi.h A bit-bang Synchronous Serial Interface Driver for STM32
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

#ifndef SSI
#define SSI

#include "ch.h"
#include "hal.h"

// SSI Data Definitions
#define SSI_LENGTH 13		//13 bit resolution
#define SSI_BUFFER_SIZE	5 	// 5 Word output buffer size

// SSI Pin Definitions
#define SSI_SIGNAL_PORT GPIOA	// SSI Data/Clock I/O Port
#define SSI_CLK_PIN 	4		// SSI Clock out pin (PA4)
#define SSI_DATA_PIN 	3		// SSI Data input pin (PA3)
#define SSI_CTL_PORT 	GPIOB	// Interface control signal pins
#define SSI_TXEN_PIN	5		// Transceiver Tx Enable (PB5)
#define SSI_NRXEN_PIN	6		// Transceiver ~Rx Enable (PB6)
#define SSI_PWREN_PIN	7		// Encoder Power Enable (PB7)

// SSI Control Macros
#define SSI_TXEN_ON()		palSetPad(SSI_CTL_PORT, SSI_TXEN_PIN)
#define SSI_TXEN_OFF()		palClearPad(SSI_CTL_PORT, SSI_TXEN_PIN)
#define SSI_RXEN_ON()		palClearPad(SSI_CTL_PORT, SSI_NRXEN_PIN)
#define SSI_RXEN_OFF()		palSetPad(SSI_CTL_PORT, SSI_NRXEN_PIN)

// SSI Modes
#define SSI_MODE_STREAM	1	// Continuous stream mode
#define SSI_MODE_POLL	0	// Polled mode

// Encoder Power States
#define ENCODER_OFF 0
#define ENCODER_ON  1

void ssi_encoder_power(uint8_t pwr_state);
uint32_t ssi_read(void);
void ssi_set_mode(uint8_t mode);
void ssi_increase_delay(void);
void ssi_decrease_delay(void);
bool ssi_poll(void);

uint8_t ssi_data_in_queue(void);
uint32_t ssi_get_data(void);
msg_t ssi_main(void * arg);

#endif