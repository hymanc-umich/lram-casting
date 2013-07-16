/*
 * aeat9000.h
 *
 *  Created on: May 1, 2013
 *      Author: cody
 */

#ifndef STM32F1
#define STM32F1
#endif

#ifndef AEAT9000_H_
#define AEAT9000_H_

#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/nvic.h>
#include "ssi.h"

// I/O Port/Pin Defines
#define LOCTEST_PORT GPIOB
#define LOCTEST_PIN GPIO0
#define TILTOUT_PORT GPIOB
#define TILTOUT_PIN GPIO1
#define ZERORST_PORT GPIOB
#define ZERORST_PIN GPIO3
#define MSBINV_PORT GPIOB
#define MSBINV_PIN GPIO4
#define ENCRST_PORT GPIOA
#define ENCRST_PIN GPIO11
#define ENCERR_PORT GPIOA
#define ENCERR_PIN GPIO15

void aeat9k_initialize(void);
u32 aeat9k_read_blocking(void);
void aeat9k_read_buffer(encoder_value_t * buf);
void aeat9k_zero(void);
void aeat9k_reset(void);
bool aeat9k_get_msb_inv(void);
void aeat9k_set_msb_inv(bool inv_state);
u8 aeat9k_error(void);
// u8 aeat9k_loctest(void); // Convert for ADC read

#endif /* AEAT9000_H_ */
