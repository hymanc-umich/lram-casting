/*  bluetooth.c AirSSI Module Bluetooth Communication Manager
    Copyright (C) 2013 Cody Hyman, OSU Lab for Robotics & Applied Mechanics

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "bluetooth.h"
#include "pwr_control.h"
#include "ssi.h"

/**
 * @brief Data output format type enumeration for ASCII or BINARY formats
 */
enum DATA_FORMAT
{
	ASCII,
	BINARY
};
volatile enum DATA_FORMAT format = ASCII; // Default to ascii format

volatile bool tx_end_flag = FALSE;
volatile bool rx_end_flag = TRUE;

volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_buffer_head = 0;
volatile uint8_t rx_buffer_tail = 0;
volatile uint8_t rx_buffer_count = 0;

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) {
  (void)uartp;
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {
	tx_end_flag = TRUE;
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;
}

/**
 * @brief Command parser for single-byte commands
 * @param uint8_t command Command character
 * @return bool Command is valid
 */
bool bt_parse_command(uint8_t command)
{
	//led_toggle();
	led_blink(&bt_parse_command);
	switch(command)
		{
		case BT_CMD_REQUEST_POSITION:
		{
			ssi_poll();	// Initiate SSI Data Request
			return TRUE;
		}
		case BT_CMD_START_STREAM:
		{
			uartStartSend(&UARTD1, 17, "SSI-STREAM MODE\r\n");
			ssi_set_mode(SSI_MODE_STREAM); // Start SSI Stream mode
			return TRUE;
		}
		case BT_CMD_EXIT_STREAM:
		{
			uartStartSend(&UARTD1,15, "SSI-POLL MODE\r\n");
			ssi_set_mode(SSI_MODE_POLL); // Exit SSI stream mode
			return TRUE;
		}
		case BT_CMD_INCR_STREAM_DELAY:
		{
			uartStartSend(&UARTD1, 15, "STREAM DELAY+\r\n"); 
			ssi_increase_delay();		// Increase stream delay by 1ms
			return TRUE;
		}
		case BT_CMD_DECR_STREAM_DELAY:
		{
			uartStartSend(&UARTD1, 15, "STREAM DELAY-\r\n"); 
			ssi_decrease_delay();		// Decrease stream delay by 1ms
			return TRUE;
		}
		case BT_CMD_ENABLE_ENC_PWR:
		{
			uartStartSend(&UARTD1, 12, "ENCODER ON\r\n");
			ssi_encoder_power(ENCODER_ON); // Turn on 5V encoder power
			return TRUE;
		}
		case BT_CMD_DISABLE_ENC_PWR:
		{
			uartStartSend(&UARTD1, 13, "ENCODER OFF\r\n");
			ssi_encoder_power(ENCODER_OFF); // Turn off 5V encoder power
			return TRUE;
		}
		case BT_CMD_GET_BATT_VOLT:
		{
			pwr_check_battery(TRUE);//Read and return raw battery voltage
			return TRUE;
		}
		case BT_CMD_CHANGE_FORMAT:
		{
			if(format == ASCII)
			{
				format = BINARY; 	// Change format to binary
				uartStartSend(&UARTD1, 16, "FORMAT: BINARY\r\n");
			}
			else
			{
				format = ASCII;		// Change formate to ASCII
				uartStartSend(&UARTD1, 15, "FORMAT: ASCII\r\n");
			}
			return TRUE;
		}
		/*case BT_CMD_HELP:
		{
			uartStartSend(&UARTD1,19,"\r\n==AirSSI Help==\r\n");
			chThdSleepMicroseconds(50);
			uartStartSend(&UARTD1,32,"'r' - Request encoder position\r\n");
			chThdSleepMicroseconds(50);
			uartStartSend(&UARTD1,25,"'s' - Start stream mode\r\n");
			chThdSleepMicroseconds(50);
			uartStartSend(&UARTD1,24,"'e' - Exit stream mode\r\n");
			chThdSleepMicroseconds(50);
			uartStartSend(&UARTD1,24,"'p' - Encoder power on\r\n");
			chThdSleepMicroseconds(50);
			uartStartSend(&UARTD1,25,"'d' - Encoder power off\r\n");
			chThdSleepMicroseconds(50);
			uartStartSend(&UARTD1,36,"'f' - Encoder output format toggle\r\n");
			chThdSleepMicroseconds(50);
			uartStartSend(&UARTD1,36,"'+' - Increase stream delay (50us)\r\n");
			chThdSleepMicroseconds(50);
			uartStartSend(&UARTD1,36,"'-' - Decrease stream delay (50us)\r\n");
			chThdSleepMicroseconds(50);
			uartStartSend(&UARTD1,28,"'v' - Read battery voltage\r\n");
		}*/
		default:
			return FALSE;
		}
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c) {

  (void)uartp;
  uint8_t rx_byte = (uint8_t) c;
  bt_parse_command(rx_byte); 
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {

  (void)uartp;
  rx_end_flag = FALSE;
}

/*
 * UART Configuration parameter struct
 */
static UARTConfig uart_cfg_1 = {
  txend1,
  txend2,
  rxend,
  rxchar,
  rxerr,
  921600,
  0,
  USART_CR2_LINEN,
  0
};

/**
 * @brief Initializes Bluetooth serial port I/O and sends out a startup message
 * @param UARTDriver *uartp ChibiOS HAL UART driver
 */
void bt_init(UARTDriver *uartp)
{
	// USART I/O setup
	palSetPadMode(USART1_PORT, USART1_TX_PIN, PAL_MODE_ALTERNATE(1));
	palSetPadMode(USART1_PORT, USART1_RX_PIN, PAL_MODE_ALTERNATE(1));
	uartStart(uartp, &uart_cfg_1);
	uartStartSend(uartp, 13, "AirSSI v2.0\r\n");
}

/**
 * @brief Convenience function to change data format between ASCII or BINARY
 * @param enum DATA_FORMAT d Data format to use (ASCII or BINARY)
 */
void bt_set_data_format(enum DATA_FORMAT d)
{
	format = d;
}

/**
 * @brief Helper function for base 10 int to char conversion
 * @param uint32_t in Integer (0-9) to convert to char
 * @return char ASCII character representation of input digit
 */
static char numChar(uint32_t in)
{
	switch(in)
	{
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		default:
			return 'n';
	}
}

/**
 * @brief A function to send formatted positions via Bluetooth in either binary or ASCII
 * @param uint32_t position The 17-bit position value to send back via Bluetooth
 * @return Successful send initiated
 */
bool bt_send_position(uint32_t position)
{
	tx_end_flag = FALSE;
	if(format == BINARY)	// Binary output format (5 bytes)
	{
		uint8_t bytes[4];
		// Header bytes
		bytes[0] = 0xAA;
		bytes[1] = 0xAA;
		// Formatted position (binary)
		bytes[2] = (uint8_t) (position >> 8);
		bytes[3] = (uint8_t) position;
		uartStartSend(&UARTD1, 4, bytes);	// Write to serial port
		return TRUE;
	}
	else if(format == ASCII)	// ASCII output format (10 bytes)
	{
		char out_string[8];
		// Header bytes
		out_string[0] = 'P';
		out_string[1] = ':';
		// Formatted position (ASCII, base 10)
		out_string[2] = numChar((uint8_t)((position % 10000)/1000));
		out_string[3] = numChar((uint8_t)((position % 1000)/100));
		out_string[4] = numChar((uint8_t)((position % 100)/10));
		out_string[5] = numChar((uint8_t) (position % 10));
		// Newline
		out_string[6] = '\r';
		out_string[7] = '\n';
		uartStartSend(&UARTD1, 8, out_string);
		return TRUE;
	}
	return FALSE;
}

/**
 * @brief Formats raw battery ADC values into voltages and sends them via Serial
 * @param uint16_t voltage Raw voltage ADC value
 * @return bool Status of read and send (currently not checked)
 */
bool bt_send_voltage(uint16_t voltage)
{
	char out_string[16]; //"Battery:11.666V\r\n
	out_string[0] = 'B';
	out_string[1] = 'A';
	out_string[2] = 'T';
	out_string[3] = 'T';
	out_string[4] = 'E';
	out_string[5] = 'R';
	out_string[6] = 'Y';
	out_string[7] = ':';
	
	uint32_t intermediate_voltage = (((uint32_t) voltage) * CONVERSION_FACTOR)/1E5;
	uint16_t battv_integer = intermediate_voltage / 100;
	uint16_t battv_fractional = intermediate_voltage % 100; 
	if(battv_integer > 10)
	{
		out_string[8] = numChar(battv_integer / 10);
		out_string[9] = numChar(battv_integer % 10);
	}
	else
	{
		out_string[8] = '0';
		out_string[9] = numChar(battv_integer);
	}
	out_string[10] = '.';
	out_string[11] = numChar(battv_fractional / 10);
	out_string[12] = numChar(battv_fractional % 10);
	
	out_string[13] = 'V';
	out_string[14] = '\r';
	out_string[15] = '\n';
	uartStartSend(&UARTD1, 16, out_string); // Send things out*/
	return TRUE;
}

/**
 * @brief Accessor for the position tx state 
 * @return bool The serial position readout complete flag state
 */
bool bt_get_tx_state(void)
{
	return tx_end_flag;
}