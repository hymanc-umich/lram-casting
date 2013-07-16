/*
 * rm_encoder.h
 * RLS Magnetic SSI Encoder Interface for the ATXmega D4 
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 


#ifndef RM_ENCODER_H_
#define RM_ENCODER_H_

#include <avr/io.h>
#include <util/delay.h>
#include "SSI/ssi_rls.h"

#define ENC_EN_PORT PORTC	// Encoder power switch port
#define ENC_EN_bm 1<<5		// Encoder power switch pin bitmask
#define ENCODER_OFF 0		// Encoder off-state define
#define ENCODER_ON	1		// Encoder on-state define

#define ENCODER_RESET_DELAY 100	// 100ms encoder reset delay

void rm_encoder_initialize(void);
uint16_t rm_encoder_read_blocking(void);
void rm_encoder_read_non_blocking(void * nb_handler(uint16_t enc_value));
void rm_encoder_power_on(void);
void rm_encoder_power_off(void);
void rm_encoder_reset(void);
uint8_t rm_encoder_get_power_state(void);
uint16_t rm_encoder_hanlder(uint16_t readout_value);

#endif /* RM_ENCODER_H_ */
