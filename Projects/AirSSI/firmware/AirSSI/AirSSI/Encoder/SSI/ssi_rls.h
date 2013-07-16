/*
 * ssi_rls.h
 * RLS RE22 SSI Encoder Interface for the ATXmega D4 
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 

#ifndef SSI_RLS_H_
#define SSI_RLS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/* Boolean Type */
#ifndef BOOLEAN_TYPE
	#define BOOLEAN_TYPE
	typedef uint8_t bool;
	#define TRUE 1
	#define FALSE 0
#endif 

// SSI I/O Defines
#define SSI_PORT PORTC
#define SSI_CLK_PIN_bm 1<<1
#define SSI_DATA_PIN_bm 1<<2
#define SSI_TXEN_PORT PORTC
#define SSI_TXEN_PIN_bm 1<<0
#define SSI_NRXEN_PORT PORTC
#define SSI_NRXEN_PIN_bm 1<<4
#define SSI_ENC_PWR_EN_PORT PORTC
#define SSI_ENC_PWR_EN_PIN_bm 1<<5
#define SSI_TIMER TCC0

#define SSI_PERIOD 1280

// Encoder Power States
#define SSI_POWER_ON 1
#define SSI_POWER_OFF 0

// SSI Differential Clock Transmitter State
#define SSI_TX_ACTIVE 1
#define SSI_TX_INACTIVE 0

// SSI Differential Data Receiver State
#define SSI_RX_ACTIVE 1
#define SSI_RX_INACTIVE 0

#define SSI_BYTE_1 1
#define SSI_BYTE_2 2

void ssi_initialize(void);
void ssi_set_enc_handler(uint16_t (*enc_value_handler)(uint16_t enc_value));
bool ssi_start_read(void);
bool ssi_read_in_progress(void);
void ssi_set_rx_active(uint8_t ssi_rx_state);
void ssi_set_tx_active(uint8_t ssi_tx_state);

#endif /* SSI_RLS_H_ */