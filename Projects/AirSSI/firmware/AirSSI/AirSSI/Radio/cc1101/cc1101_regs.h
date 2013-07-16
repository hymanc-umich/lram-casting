/*
 * cc1101_regs.h
 * Register address definitions for CC1101 radio
 * Author: Cody Hyman
 * Copyright 2013
 * Lab for Robotics and Applied Mechanics
 * Oregon State University
 */ 

#ifndef CC1101_REGS_H_
#define CC1101_REGS_H_

/* CC1101 Register SPI Addresses */

// Configuration Registers
#define IOCFG2	0x00 // GDO2 output config
#define IOCFG1	0x01 // GDO1 output config
#define IOCFG0	0x02 // GDO0 output config
#define FIFOTHR 0x03 // FIFO threshold
#define SYNC1	0x04 // Sync word high byte
#define SYNC0	0x05 // Sync word low byte
#define PKTLEN	0x06 // Packet length
#define	PKTCTRL1 0x07 // Packet control register 1
#define	PKTCTRL0 0x08 // Packet control register 0
#define	ADDR	0x09 // Address register
#define	CHANNR	0x0A // Channel register
#define	FSCTRL1	0x0B // Freq. synth control 1
#define	FSCTRL0	0x0C // Freq. synth control 0
#define	FREQ2	0x0D // Frequency control high byte
#define	FREQ1	0x0E // Frequency control middle byte
#define	FREQ0	0x0F // Frequency control low byte
#define	MDMCFG4	0x10 // Modem configuration 4
#define	MDMCFG3	0x11 // Modem configuration 3
#define	MDMCFG2	0x12 // Modem configuration 2
#define	MDMCFG1	0x13 // Modem configuration 1
#define	MDMCFG0	0x14 // Modem configuration 0
#define	DEVIATN	0x15 // Modem deviation setting
#define	MCSM2	0x16 // Main radio control state machine cfg 2
#define	MCSM1	0x17 // Main radio control state machine cfg 1
#define	MCSM0	0x18 // Main radio control state machine cfg 0
#define	FOCCFG	0x19 // Freq. offset compensation cfg
#define	BSCFG	0x1A // Bit synchronization cfg
#define	AGCCTRL2	0x1B // AGC control 2
#define	AGCCTRL1	0x1C // AGC control 1
#define	AGCCTRL0	0x1D // AGC control 0
#define	WOREVT1	0x1E // Event 0 timeout high byte
#define	WOREVT0	0x1F // Event 0 timeout low byte
#define	WORCTRL	0x20 // Wake on radio control
#define	FREND1	0x21 // Front-end RX configuration
#define	FREND0	0x22 // Front-end TX configuration
#define	FSCAL3	0x23 // Frequency synth calibration 3
#define	FSCAL2	0x24 // Frequency synth calibration 2
#define	FSCAL1	0x25 // Frequency synth calibration 1
#define	FSCAL0	0x26 // Frequency synth calibration 0
#define	RCCTRL1	0x27 // RC oscillator configuration 1
#define	RCCTRL0	0x28 // RC oscillator configuration 0
#define	FSTEST	0x29 // Frequency synth calibration control
#define	PTEST	0x2A // Production test
#define	AGCTEST	0x2B // AGC test
#define	TEST2	0x2C // Various test settings 2 
#define	TEST1	0x2D // Various test settings 1
#define	TEST0	0x2E // Various test settings 0

// Status Registers
#define PARTNUM	0x30 // Part number for CC1101
#define VERSION 0x31 // Current version number
#define FREQEST 0x32 // Frequency offset estimate
#define LQI		0x33 // Demodulator estimate for link quality
#define RSSI	0x34 // Received signal strength indication
#define MARCSTATE	0x35 // Control state machine state
#define WORTIME1	0x36 // High byte of WOR timer
#define WORTIME0	0x37 // Low byte of WOR timer
#define PKTSTATUS	0x38 // Current GDOx status and packet status
#define VCO_VC_DAC	0x39 // Current setting from PLL cal module
#define TXBYTES	0x3A // Overflow and # of Tx FIFO bytes
#define RXBYTES	0x3B // Overflow and # of Rx FIFO bytes
#define RCCTRL1_STATUS	0x3C // Last RC osc. cal result
#define RCCTRL0_STATUS	0x3D // Last RC osc. cal result
#define FIFO 0x3F

// Command Strobes
#define SRES	0x30 // Reset Chip
#define SFSTXON	0x31 // Enable and cal freq. synth.
#define SXOFF	0x32 // Turn off crystal oscillator
#define SCAL	0x33 // Calibrate freq. synth and turn off
#define SRX		0x34 // Enable Rx (Cal must be done first after IDLE)
#define STX		0x35 // Enable Tx (Cal must be performed first)
#define SILDE	0x36 // Exit Rx/Tx, turn off synth, exit WOR mode
#define SWOR	0x38 // Start auto Rx Polling sequency (WOR)
#define SPWD	0x39 // Enter power down mode when CSn goes high
#define SFRX	0x3A // Flush Rx FIFO (IDLE or RX OVF states only)
#define SFTX	0x3B // Flush Tx FIFO (IDLE or TX UNF states only)
#define SWORRST	0x3C // Reset RTC to EVENT1 value
#define SNOP	0x3D // No operation (use to get status byte)
#endif /* CC1101_REGS_H_ */