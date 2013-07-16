/*
 * debug_serial.c
 * Asynchronous blocking serial debug port driver
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 

#include "debug_serial.h"

/*
 * @brief Initializes the debug serial port for Tx/Rx 8N1 operation at 115.2kbps w/ a 4MHz system clock
 */
void debug_serial_initialize(void)
{
	PORTD.DIRSET = (1<<3);
	//115200 baud BSEL = -7, BSCALE = 0x1001
	uint16_t bsel = 0x1001;
	int8_t bscale = ((~7)+1) & 0x0F; // Two's compliment
	USARTD0.BAUDCTRLA = (uint8_t) (bsel & 0xFF);
	USARTD0.BAUDCTRLB = (uint8_t) ((bscale << 4) | ((bsel & 0x0F00) >> 8));
	USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	USARTD0.CTRLC = USART_CHSIZE_8BIT_gc;
}

/*
 * @brief Sends a single byte over the debug serial port
 * @param uint8_t byte The byte to send over the debug serial port
 */
void debug_serial_send_byte(uint8_t byte)
{
	while(!(USARTD0.STATUS & USART_TXCIF_bm))
	{
		__asm__("NOP");	// Wait for prior transmissions to complete
	}
	USARTD0.DATA = byte;
}

/*
 * @brief Checks to see if any data has been received by the debug serial port
 * @return uint8_t 1 if there is data in the Rx buffer, 0 if there is not 
 */
uint8_t debug_serial_has_data(void)
{
	uint8_t has_data = USARTD0.STATUS & USART_RXCIF_bm;
	return has_data;
}

/*
 * @brief Reads the Rx buffer
 * @return uint8_t The byte value read from the debug serial port Rx buffer
 */
uint8_t debug_serial_read_byte(void)
{
	if(!(USARTD0.STATUS & USART_RXCIF_bm))
	{
		return 0xFF;
	}
	else
		return USARTD0.DATA;
}