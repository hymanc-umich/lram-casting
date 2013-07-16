/*
 * CFile1.c
 *
 * Created: 4/20/2013 12:25:43 PM
 *  Author: Cody
 */ 

#include "spi_master.h"


volatile void (*spi_irq_callback)(uint8_t spi_read_data);

/* Rising edge sample */
void spi_master_initialize(void)
{
	cli();
	// Set up SPI for sample on rising edge, clock idle low
	SPI.CTRL = SPI_MASTER_bm | SPI_ENABLE_bm | SPI_CLK2X_bm;
	SPI.INTCTRL = SPI_INTLVL_MED_gc;
	SPI_PORT.DIRSET = SPI_MOSI_PIN_bm | SPI_SCK_PIN_bm | SPI_NCS_PIN_bm;
	// Set up Tx/Rx buffers
	spi_tx_buffer.capacity = SPI_TX_BUFFER_DEPTH;
	spi_rx_buffer.capacity = SPI_RX_BUFFER_DEPTH;
	sei();
}

/* @brief Initializes a SPI Master Write
 * @param uint8_t byte Byte to write out on SPI MOSI line
 * @return bool Successful write
 */
bool spi_master_send_byte(uint8_t byte)
{
	// Check for unfinished write
	SPI.DATA = byte; // Start write
	return TRUE; // Return true if transmission start successful
}

/*
 *
 *
 */
void spi_master_set_ncs(uint8_t ncs_state)
{
	if(ncs_state)
	{
		SPI_PORT.OUTSET = SPI_NCS_PIN_bm;
	}
	else
	{
		SPI_PORT.OUTCLR = SPI_NCS_PIN_bm;
	}
}

/*
 *
 *
 */
void spi_master_set_irq_callback(void (*irq_callback_ptr)(uint8_t spi_read_data))
{
	spi_irq_callback = irq_callback_ptr;
}
