/*
 * ssi.h
 *
 *  Created on: May 1, 2013
 *      Author: cody
 */

#ifndef STM32F1
#define STM32F1
#endif

#ifndef SSI_H_
#define SSI_H_

#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/f1/rcc.h>

#define SSI_TIMER TIM2
#define SSI_GPIO GPIOA
#define SSI_CLK_PIN GPIO8
#define SSI_DATA_PIN GPIO10
#define SSI_NCS_PIN GPIO9

#define TIMER_MAX 23 // For 250kHz operation @ 24MHz

#define SSI_IDLE 0
#define SSI_START 1
#define SSI_CLK_HIGH 2
#define SSI_CLK_LOW 3

struct encoder_value
{
	u32 value;
	bool ready_flag;
};

typedef struct encoder_value encoder_value_t;

void ssi_initialize(u16 clock_freq);
void ssi_start_read(u8 nbits);
void ssi_bit_handler(u8 bit);
bool ssi_data_ready(void);
u32 ssi_read_data_buffer();
void ssi_set_ext_buffer(encoder_value_t * buf);

#endif /* SSI_H_ */
