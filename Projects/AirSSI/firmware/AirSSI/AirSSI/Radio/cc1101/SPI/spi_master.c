/*
 * spi_master.c
 * ATxmega D4 simple blocking SPI driver
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */  

#include "spi_master.h"

/*
 * @brief Initializes the defined SPI port for SPI master operation 
 *
 */
void spi_master_initialize(void)
{
	// Set up SPI for sample on rising edge, clock idle low
	SPI_PORT.DIRSET = SPI_MOSI_PIN_bm | SPI_SCK_PIN_bm | SPI_NCS_PIN_bm;
	SPI_PORT.OUTSET = SPI_NCS_PIN_bm;
	SPI.CTRL = SPI_MASTER_bm | SPI_ENABLE_bm | SPI_PRESCALER_DIV4_gc | SPI_CLK2X_bm;
	SPI.DATA = 0x00;
}

/* @brief Initializes a SPI Master Write
 * @param uint8_t byte Byte to write out on SPI MOSI line
 * @return uint8_t Read master in value
 */
uint8_t spi_master_transact(uint8_t send_byte)
{
	// Check for unfinished write and 
	SPI.DATA = send_byte; // Start write
	while(!(SPI.STATUS & SPI_IF_bm))
	{__asm__("NOP");}
	uint8_t out_data = SPI.DATA;
	return out_data;; // Return read value
}

/*
 * @brief Sets the chip select state of SPI master
 * @param uint8_t ncs_state The state of the chip select pin to set
 */
void spi_master_set_ncs(uint8_t ncs_state)
{

	if(ncs_state == 1)
	{
		PORTD.OUTSET = SPI_NCS_PIN_bm;
	}
	else
	{
		PORTD.OUTCLR = SPI_NCS_PIN_bm;
	}
}

