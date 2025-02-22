/*
 * ssi.c
 *
 *  Created on: May 1, 2013
 *      Author: cody
 */


#include "ssi.h"

volatile u32 ssi_buffer;
volatile u8 ssi_counter;
volatile u8 ssi_state;
volatile bool ssi_data_ready_flag;
volatile bool ext_buffer_flag;
volatile encoder_value_t * ext_buffer;

// TODO: User set clock frequency
/**
 * @brief Initializes the AEAT9000 SSI Link
 * @param u16 clk_speed SSI Clock Speed in 100bps LSB units
 */
void ssi_initialize(u16 clk_speed)
{
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN); // Initialize timer clk
	// Initialize I/O Pins
	gpio_set_mode(SSI_GPIO, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, SSI_CLK_PIN);
	gpio_set_mode(SSI_GPIO, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, SSI_DATA_PIN);
	gpio_set_mode(SSI_GPIO, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, SSI_NCS_PIN);
	// Initialize clock timer on TIM1
	TIM_DIER(SSI_TIMER) = TIM_DIER_UIE;
	TIM_PSC(SSI_TIMER) = 1; // 24MHz Clock
	TIM_ARR(SSI_TIMER) = TIMER_MAX; // Update at 500kHz
	// Initialize NVIC
	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 1);
	ssi_state = SSI_IDLE;
	ssi_data_ready_flag = false;
}

/**
 * @brief Trigger function to start a 2 wire SSI read of arbitrary length
 * @param u7 nbits The number of bits to read
 */
void ssi_start_read(u8 nbits)
{
	if(ext_buffer_flag == true)
	{
		ext_buffer->ready_flag = 0;
	}
	ssi_counter = nbits;
	ssi_state = SSI_START;
	TIM_CNT(SSI_TIMER) = 0;
	TIM_CR1(SSI_TIMER) |= TIM_CR1_CEN;// Start counter
}

/**
 * Timer 2 interrupt service routine
 */
void tim2_isr(void)
{
	TIM_SR(SSI_TIMER) &= ~TIM_SR_UIF;// Clear Interrupt Flag
	// Check state and run correct handler action
	switch(ssi_state)
	{
	case(SSI_START): // SSI Start of Frame
	{
		gpio_clear(SSI_GPIO, SSI_CLK_PIN);// Set the clock low
		ssi_state = SSI_CLK_LOW; // Change SSI state
		break;
	}
	case(SSI_CLK_HIGH): // SSI Clock High-to-Low
	{
		gpio_clear(SSI_GPIO, SSI_CLK_PIN);// Set clock low
		u8 input_bit = GPIO_IDR(SSI_GPIO) & SSI_DATA_PIN;// Read bit on pin 10
		ssi_bit_handler(input_bit); // Call bit handler
		ssi_state = SSI_CLK_LOW;
		break;
	}
	case(SSI_CLK_LOW): // SSI Clock Low-to-High
	{
		// Set clock high
		gpio_set(SSI_GPIO, SSI_CLK_PIN);
		if(ssi_counter == 0)
		{
			ssi_state = SSI_IDLE;
			ssi_data_ready_flag = true;
			TIM_CR1(SSI_TIMER) &= ~TIM_CR1_CEN; // Turn off timer
		}
		else
			ssi_state = SSI_CLK_HIGH;
		break;
	}
	case(SSI_IDLE): // SSI Idle (Not Used)
			break;
	default:
		break;
	}
}

void ssi_bit_handler(u8 bit)
{
	ssi_counter--; // Decrement bit Counter
	ssi_buffer = ssi_buffer << 1; // Shift buffer
	if(bit == 1)
	{
		ssi_buffer |= 1; // Add 1 to LSB
	}
	if((ssi_counter == 0) && (ext_buffer_flag == true))
	{
		ext_buffer->value = ssi_buffer;
		ext_buffer->ready_flag = true;
	}
	// Set Timer
	TIM_CNT(SSI_TIMER) = 0;
	TIM_CR1(SSI_TIMER) |= TIM_CR1_CEN;
}

bool ssi_data_ready(void)
{
	return ssi_data_ready_flag;
}

u32 ssi_read_data_buffer()
{
	if(ssi_data_ready())
	{
		ssi_data_ready_flag = false;
		return ssi_buffer;
	}
	else
		return 0xFFFF;
}

void ssi_set_ext_buffer(encoder_value_t * buf)
{
	ext_buffer = buf;
	ext_buffer_flag = true;
}
