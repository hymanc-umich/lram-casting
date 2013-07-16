/*
 * cc1101.h
 * CC1101 SPI based radio driver
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */  

#include "cc1101.h"
#include "spi/spi_master.h"
#include "a1101R09_setup.h"

/* CC1101 Packet Structure
	Command Strobe:				
		Header Strobe 
	R/W Registers: 
		Header (Reg) |  Data
	R/W Conesecutive Registers
		Header (Regn)| Data (n) | Data (n+1)| Data (n+2) | ...
	R/W N+1 bytes from Tx/Rx:
		Header (FIFO)| Data (0) | Data (1)	| Data (2) | ... | Data (n-1) | Data (n) 
*/

volatile uint8_t cc1101_status;
/*
 * @brief Initializes the CC1101 for the A1101R09 to operate at 500kbps
 */
bool cc1101_initialize()
{
	// Initialize SPI (Sample on rising, clk idle low)
	spi_master_initialize();
	
	// Reset CC1101
	cc1101_command_strobe(SRES);
	_delay_ms(10);	// Wait for reset
	
	// CC1101 Configuration Register Spree
	cc1101_write_register(IOCFG0, IOCFG0_DEFAULT);
	cc1101_write_register(IOCFG1, IOCFG1_DEFAULT);
	cc1101_write_register(IOCFG2, IOCFG2_DEFAULT);
	cc1101_write_register(FIFOTHR, FIFOTHR_DEFAULT);
	cc1101_write_register(SYNC0, SYNC1_DEFAULT);
	cc1101_write_register(SYNC1, SYNC2_DEFAULT);
	cc1101_write_register(PKTLEN, PKTLEN_DEFAULT);
	cc1101_write_register(PKTCTRL0, PKTCTRL0_DEFAULT);
	cc1101_write_register(PKTCTRL1, PKTCTRL1_DEFAULT);
	cc1101_write_register(ADDR, ADDR_DEFAULT);
	cc1101_write_register(CHANNR, CHANNR_DEFAULT);
	cc1101_write_register(FSCTRL0, FSCTRL0_DEFAULT);
	cc1101_write_register(FSCTRL1, FSCTRL1_DEFAULT);
	cc1101_write_register(FREQ0, FREQ0_DEFAULT);
	cc1101_write_register(FREQ1, FREQ1_DEFAULT);
	cc1101_write_register(FREQ2, FREQ2_DEFAULT);
	cc1101_write_register(MDMCFG0, MDMCFG0_DEFAULT);
	cc1101_write_register(MDMCFG1, MDMCFG1_DEFAULT);
	cc1101_write_register(MDMCFG2, MDMCFG2_DEFAULT);
	cc1101_write_register(MDMCFG3, MDMCFG3_DEFAULT);
	cc1101_write_register(MDMCFG4, MDMCFG4_DEFAULT);
	cc1101_write_register(DEVIATN, DEVIATN_DEFAULT);
	cc1101_write_register(MCSM0, MCSM0_DEFAULT);
	cc1101_write_register(MCSM1, MCSM1_DEFAULT);
	cc1101_write_register(MCSM2, MCSM2_DEFAULT);
	cc1101_write_register(FOCCFG, FOCCFG_DEFAULT);		// Anaren 500k spec
	cc1101_write_register(BSCFG, BSCFG_DEFAULT);		// Anaren 500k spec
	cc1101_write_register(AGCCTRL0, AGCCTRL0_DEFAULT);	// Anaren 500k spec
	cc1101_write_register(AGCCTRL1, AGCCTRL1_DEFAULT);	// Anaren 500k spec
	cc1101_write_register(AGCCTRL2, AGCCTRL2_DEFAULT);	// Anaren 500k spec
	cc1101_write_register(WOREVT0, WOREVT0_DEFAULT);	// Anaren 500k spec
	cc1101_write_register(WOREVT1, WOREVT1_DEFAULT);	// Anaren 500k spec
	cc1101_write_register(WORCTRL, WORCTRL_DEFAULT);	// Anaren 500k spec
	cc1101_write_register(FREND0, FREND0_DEFAULT);		// Anaren 500k spec
	cc1101_write_register(FREND1, FREND1_DEFAULT);		// Anaren 500k spec
	cc1101_write_register(FSCAL0, FSCAL0_DEFAULT);		// Anaren 500k spec
	cc1101_write_register(FSCAL1, FSCAL1_DEFAULT);		// Anaren 500k spec
	cc1101_write_register(FSCAL2, FSCAL2_DEFAULT);		// Anaren 500k spec
	cc1101_write_register(FSCAL3, FSCAL3_DEFAULT);		// Anaren 500k spec
	cc1101_write_register(RCCTRL0, RCCTRL0_DEFAULT);	// Anaren 500k spec
	cc1101_write_register(RCCTRL1, RCCTRL1_DEFAULT);	// Anaren 500k spec
	
	// Calibrate 
	cc1101_calibrate();
	cc1101_command_strobe(SRX);
	// CC1101 command strobe to turn on
	
	return TRUE;
}

/*
 * @brief Convenience function for setting the address of the CC1101
 * @param uint8_t address
 */
void cc1101_set_address(uint8_t address)
{
	cc1101_write_register(ADDR, address);
}


uint8_t cc1101_get_address(void)
{
	uint8_t ret_val;
	cc1101_read_register(ADDR, &ret_val);
	return ret_val;
}

/*
 * @brief Sets the currently used channel
 * @param channel The channel to set the 
 */
void cc1101_set_channel(uint8_t channel)
{
	cc1101_write_register(CHANNR, channel);
}

/*
 * @brief Returns the currently set channel
 * @return Current CC1101 channel
 */
uint8_t cc1101_get_channel(void)
{
	uint8_t ret_val;
	cc1101_read_register(CHANNR, &ret_val);
	return ret_val;
}

// Basic CC1101 Communication Utilities
/*
 * @brief Sends the specified strobe
 * @return Status byte
 */
uint8_t cc1101_command_strobe(uint8_t strobe)
{
	spi_master_set_ncs(0);							// Enable slave SPI
	uint8_t status = spi_master_transact(strobe);	// Write strobe/read status
	spi_master_set_ncs(1);							// Disable slave SPI
	cc1101_status = status;
	return status;
}

/*
 * @brief A routine to read a single register on the CC1101
 * @param uint8_t register_addr The address of the register to read
 * @param uint8_t * read_value A pointer to the buffer for the register contents
 * @return uint8_t The CC1101 status byte
 */
uint8_t cc1101_read_register(uint8_t register_addr, uint8_t * read_value)
{
	spi_master_set_ncs(0);									// Enable chip select
	uint8_t header_byte = CC1101_READ_bm | register_addr; 	// Bit 7 set to 1 read, Burst set to 0
	uint8_t status = spi_master_transact(header_byte); 	// Write header and get status
	uint8_t out_value = spi_master_transact(0x00); 			// Write 00s and read register data
	*read_value = out_value;								// Store out value to defined space
	spi_master_set_ncs(1);									// Release chip select
	cc1101_status = status;
	return status;
}

/*
 * @brief A routine to perform a burst read of CC1101 registers
 * @param uint8_t register_addr The address of the register to read
 * @param uint8_t * read_value A pointer to the buffer for the first register's constants
 * @return uint8_t The CC1101 status byte
 */
uint8_t cc1101_read_register_burst(uint8_t register_addr, uint8_t * burst_buffer, uint8_t burst_length)
{
	spi_master_set_ncs(0);					// Enable slave SPI		
	// Format header for read and burst
	uint8_t header_byte = CC1101_READ_bm | CC1101_BURST_bm | register_addr;
	uint8_t status = spi_master_transact(header_byte); // Write header and get status
	// Read Data consecutively for burst length
	uint8_t i;
	for(i = 0; i < burst_length; i++)
	{
		burst_buffer[i] = spi_master_transact(0x00); // Read data into burst buffer
	}
	spi_master_set_ncs(1);					// Disable slave SPI
	cc1101_status = status;
	return status;
}

/*
 * @brief A routine to write a single CC1101 register
 * @param uint8_t register_addr The address of the register to write
 * @param uint8_t value The value to write in the specified register
 * @return uint8_t The CC1101 status byte
 */
uint8_t cc1101_write_register(uint8_t register_addr, uint8_t value)
{
	spi_master_set_ncs(0);
	uint8_t header_byte = register_addr;
	uint8_t status;
	status = spi_master_transact(header_byte);
	spi_master_transact(value);
	spi_master_set_ncs(1);
	cc1101_status = status;
	return status;
}

/*
 * @brief A routine to burst write to multiple CC1101 registers
 * @param uint8_t register_addr The address of the register to write
 * @param uint8_t value The value to write in the specified register
 * @return uint8_t The CC1101 status byte
 */
uint8_t cc1101_write_register_burst(uint8_t register_addr, uint8_t * burst_values, uint8_t burst_length)
{
	spi_master_set_ncs(0);			// Enable slave SPI
	uint8_t header_byte = CC1101_BURST_bm | register_addr;	// Format header for burst write
	uint8_t status = spi_master_transact(header_byte);		// Send header
	// Write consecutive bytes from the burst buffer for burst length
	uint8_t i;
	for(i = 0; i < burst_length; i++)
	{
		spi_master_transact(burst_values[i]); // Write burst values
	}
	spi_master_set_ncs(1);			// Disable slave SPI
	cc1101_status = status;
	return status;
}

/*
 *
 *
 */
bool cc1101_transmit_burst(uint8_t * burst, uint8_t burst_length)
{
	uint8_t status;
	cc1101_command_strobe(STX);
	spi_master_set_ncs(0);
	
	spi_master_set_ncs(1);
	cc1101_status = status;
}

/*
 *
 *
 */
bool c1101_transmit_byte(uint8_t byte)
{
	uint8_t status;
	status = cc1101_command_strobe(STX);
	// Check for transmit mode
	spi_master_set_ncs(0);
	spi_master_transact(FIFO);
	spi_master_transact(byte);
	spi_master_set_ncs(1);
	// Wait for transmit to complete
	status = cc1101_command_strobe(SRX); // Return to Rx
	cc1101_status = status;
	return TRUE;
}

/*
 *
 *
 */
bool c1101_receive_byte(uint8_t * rx_buffer)
{
	uint8_t status;
	cc1101_command_strobe(SRX);
	spi_master_set_ncs(0);
	spi_master_transact(FIFO | CC1101_READ_bm);
	*rx_buffer = spi_master_transact(0x00);
	spi_master_set_ncs(1);
	cc1101_status = status;
	return TRUE;
}

/*
 *
 *
 */
bool cc1101_receive_burst(uint8_t * rx_buffer, uint8_t burst_length)
{
	uint8_t status;
	cc1101_command_strobe(SRX);
	spi_master_set_ncs(0);
	
	spi_master_set_ncs(1);
	cc1101_status = status;
	return TRUE;
}	

/*
 * @brief Updates and returns the CC1101 status
 * @return uint8_t CC1101 status byte
 */ 
uint8_t cc1101_get_status(void)
{
	uint8_t status;
	spi_master_set_ncs(0);
	cc1101_command_strobe(SNOP);
	spi_master_set_ncs(1);
	return cc1101_status;
}

void cc1101_calibrate(void)
{
	uint8_t cal_flag = 1;
	spi_master_set_ncs(0);
	cc1101_command_strobe(SCAL);
	_delay_us(250);
	while(cal_flag != 0)
	{
		cc1101_command_strobe(SNOP);
		if((cc1101_status & STATE_bm) == STATE_IDLE)
		{
			cal_flag = 0;
		}
	}
	spi_master_set_ncs(1);
}