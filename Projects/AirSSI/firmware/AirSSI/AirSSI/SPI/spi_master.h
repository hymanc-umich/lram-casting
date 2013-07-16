/*
 * spi_master.h
 *
 * Created: 4/11/2013 11:59:25 PM
 *  Author: Cody
 */ 


#ifndef SPI_MASTER_H_
#define SPI_MASTER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef BOOLEAN_TYPE
#define BOOLEAN_TYPE
	typedef uint8_t bool;
	#define TRUE 1
	#define FALSE 0
#endif

#define SPI_PORT PORTD
#define SPI SPID
#define SPI_NCS_PIN_bm 1<<4
#define SPI_MOSI_PIN_bm 1<<5
#define SPI_MISO_PIN_bm 1<<6
#define SPI_SCK_PIN_bm 1<<7

#define SPI_TX_BUFFER_DEPTH 16
#define SPI_RX_BUFFER_DEPTH 16

void spi_master_initialize(void);
bool spi_master_write_byte(uint8_t byte);
void spi_master_set_ncs(uint8_t ncs_state);
void spi_master_set_irq_callback(void (*irq_callback_ptr)(uint8_t spi_read_data));

#endif /* SPI_MASTER_H_ */