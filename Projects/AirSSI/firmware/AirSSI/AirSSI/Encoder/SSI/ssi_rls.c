/*
 * ssi_rls.c
 * RLS RE22 SSI Encoder Interface for the ATXmega D4 
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 

#include "ssi_rls.h"

// Handler function pointer

volatile uint8_t ssi_word_state; // SSI word state (upper/lower)
volatile uint16_t last_position_buffer; // Last read position
volatile uint16_t position_latch; // Position latch (previous buffer)
volatile uint16_t (*enc_value_handler_ptr)(uint16_t enc_val); // Handler callback

/* */
void ssi_initialize()
{
	cli();
	// Initialize I/O Directions
	SSI_PORT.OUTSET = SSI_CLK_PIN_bm;
	SSI_PORT.DIRSET = SSI_CLK_PIN_bm;
	SSI_TXEN_PORT.DIRSET = SSI_TXEN_PIN_bm;
	SSI_NRXEN_PORT.DIRSET = SSI_NRXEN_PIN_bm;
	//SSI_ENC_PWR_EN_PORT.DIRSET = SSI_ENC_PWR_EN_PIN_bm;
	
	// Turn on Encoder and Enable Transceiver
	ssi_set_tx_active(SSI_TX_ACTIVE);
	ssi_set_rx_active(SSI_RX_ACTIVE);
	
	// Clock Idle High
	// Data on rising edge of clock
	
	ssi_word_state = 0;
	// Set up Bit-bang control timer
	SSI_TIMER.CTRLB = 1<<4; // CCAEN
	SSI_TIMER.PER = SSI_PERIOD; // Set to 1280 for 250kbps timing
	SSI_TIMER.CCA = SSI_PERIOD / 2;
	sei();
}

/* @brief Connects a callback function to the SSI module */
void ssi_set_enc_handler(uint16_t (*enc_value_handler)(uint16_t enc_val))
{
	enc_value_handler_ptr = enc_value_handler;
}

/* @brief Starts an SSI read*/
bool ssi_start_read(void)
{
	// Check for data low to indicate unfinished read
	if(ssi_word_state == 0 && (SSI_PORT.IN & SSI_DATA_PIN_bm))
	{
		// Reset timer business
		SSI_TIMER.INTCTRLA = TC_OVFINTLVL_HI_gc;
		SSI_TIMER.INTCTRLB = TC_CCAINTLVL_HI_gc;
		// Force timer reset
		SSI_PORT.OUTCLR = SSI_CLK_PIN_bm; // Set clock low
		SSI_TIMER.CTRLA |= TC_CLKSEL_DIV1_gc; // Start timer
		ssi_word_state = 1;
		// Return a positive start
		return TRUE;
	}
	return FALSE; // Return a false start
}

/* @brief Returns true if an SSI read is currently in progress */
bool ssi_read_in_progress(void)
{
	if(ssi_word_state == 0)
		return TRUE;
	return FALSE;
}

/* @brief Swtiches the differential receiver on and off */
void ssi_set_rx_active(uint8_t ssi_rx_state)
{
	if(ssi_rx_state == SSI_RX_ACTIVE)
		SSI_NRXEN_PORT.OUTCLR = SSI_NRXEN_PIN_bm;
	else if(ssi_rx_state == SSI_RX_INACTIVE)
		SSI_NRXEN_PORT.OUTSET = SSI_NRXEN_PIN_bm;
}

/* @brief Switches the differential transmitter between push-pull/high-Z states */
void ssi_set_tx_active(uint8_t ssi_tx_state)
{
	if(ssi_tx_state == SSI_TX_ACTIVE)
		SSI_TXEN_PORT.OUTSET = SSI_TXEN_PIN_bm;
	else if(ssi_tx_state == SSI_TX_INACTIVE)
		SSI_TXEN_PORT.OUTCLR = SSI_TXEN_PIN_bm;
}

/* @brief Timer/Counter Overflow Vector for SSI falling edge */
ISR(TCC0_OVF_vect)
{
	ssi_word_state++;
	if(ssi_word_state == 14)
	{
		// Stop conditions
		// Clear timer interrupts
		TCC0.INTCTRLA &= ~(TC_OVFINTLVL_HI_gc);
		TCC0.INTCTRLB &= ~(TC_CCAINTLVL_HI_gc);
		TCC0.CTRLA = 0; // Disable clock
		ssi_word_state = 0;
		enc_value_handler_ptr(last_position_buffer);// Run Handler
	}
	else
		SSI_PORT.OUTCLR = SSI_CLK_PIN_bm;
}

/* @brief Timer/Counter CCA Vector for Rising Edge Interrupt*/
ISR(TCC0_CCA_vect)
{
	SSI_PORT.OUTSET = SSI_CLK_PIN_bm; // Raise clock edge
	//uint8_t new_data = (SSI_PORT.IN & SSI_DATA_PIN_BM) >> 2;// Get SSI Data pin
	if(ssi_word_state < 14)
	{
		if(!(SSI_PORT.IN & SSI_DATA_PIN_bm))
		{
			(last_position_buffer << 1) & 1;
		}
		else
		{
			last_position_buffer << 1;
		}
	}		
}