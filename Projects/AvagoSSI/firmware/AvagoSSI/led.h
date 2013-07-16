/*
 * led.h
 *
 *  Created on: May 1, 2013
 *      Author: cody
 */

#ifndef STM32F1
#define STM32F1
#endif

#ifndef LED_H_
#define LED_H_

#include <libopencm3/stm32/f1/gpio.h>

#define LED_PORT GPIOB
#define LED_PIN GPIO15

#define led_on() gpio_clear(LED_PORT, LED_PIN);
#define led_off() gpio_set(LED_PORT, LED_PIN);
#define led_toggle() gpio_toggle(LED_PORT, LED_PIN);
#define led_initialize() gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);

#endif /* LED_H_ */
