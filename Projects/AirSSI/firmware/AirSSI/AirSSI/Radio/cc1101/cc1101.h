/*
 * cc1101.h
 * CC1101 SPI based radio driver
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */  


#ifndef CC1101_H_
#define CC1101_H_

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "cc1101_regs.h"

#ifndef BOOLEAN_TYPE
#define BOOLEAN_TYPE
typedef uint8_t bool;
#define TRUE 1
#define FALSE 0
#endif

#define CC1101_WRITE_bm 0
#define CC1101_READ_bm 1<<7
#define CC1101_BURST_bm 1<<6

#define CC1101_TX_CAPACITY 16
#define CC1101_RX_CAPACITY 16

// Status Byte Bits
	#define CHIP_RDY_bm		1<<7	// Bit [7]
	#define STATE_bm		7<<6	// Bits [6:4]
	#define STATE_IDLE		0<<6
	#define STATE_RX		1<<6
	#define STATE_TX		2<<6
	#define STATE_FSTXON	3<<6
	#define STATE_CALIBRATE	4<<6
	#define STATE_SETTLING	5<<6
	#define STATE_RX_OVF	6<<6
	#define STATE_TX_UNF	7<<6
	#define FIFO_BYTES_AVAIL_bm 0x0F // Bits [3:0]

// Read/Burst bitmasks
#define CC1101_READ_bm	1<<7
#define CC1101_BURST_bm 1<<6

bool cc1101_initialize(void);
void cc1101_set_address(uint8_t address);
uint8_t cc1101_get_address(void);
void cc1101_set_channel(uint8_t channel);
uint8_t cc1101_get_channel(void);

void cc1101_set_io_cfg(uint8_t cfg_property, uint8_t value);
bool cc1101_sleep(void);
bool cc1101_idle(void);
bool cc1101_wake(void);

/* Register Read/Write Handlers */
uint8_t cc1101_command_strobe(uint8_t strobe);
uint8_t cc1101_read_register(uint8_t register, uint8_t * read_buffer);
uint8_t cc1101_read_register_burst(uint8_t register, uint8_t * read_buffer, uint8_t burst_length);
uint8_t cc1101_write_register(uint8_t register, uint8_t value);
uint8_t cc1101_write_register_burst(uint8_t register, uint8_t * value, uint8_t burst_length);

/* Tx/Rx FIFO Access Functions */
bool cc1101_transmit_burst(uint8_t * burst, uint8_t burst_length);
bool cc1101_transmit_byte(uint8_t byte);
bool cc1101_receive_byte(uint8_t * rx_buffer);
bool cc1101_receive_burst(uint8_t * rx_buffer, uint8_t burst_length);

uint8_t cc1101_get_status(void);

void cc1101_calibrate(void);

#endif /* CC1101_H_ */