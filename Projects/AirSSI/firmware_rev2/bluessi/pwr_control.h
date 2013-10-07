/*  pwr_control.c AirSSI Module Power Control Manager
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

#ifndef PWR_CONTROL_H
#define PWR_CONTROL_H

#include "ch.h"
// ADC port layout
#define BATTERY_ADC_PORT GPIOA
#define BATTERY_ADC_PIN 0
#define BATTERY_ADC_CHANNEL 0 	
// LED port layout
#define LED_PORT GPIOB
#define LED_PIN	1
// Battery thresholds
#define BATT_LOW_THRESHOLD 2116// 12 bit low battery warning at 6V (3V/cell)
#define BATT_MIN_THRESHOLD 1975 // 12 bit cutoff for 5.6V(2.8V/cell)
// Magic conversion factor
#define CONVERSION_FACTOR 28354
//#define CONVERSION_FACTOR 283474 // Conversion factor magic number for actual voltage
//#define CONVERSION_FACTOR 257704
//#define CONVERSION_FACTOR 391194
// Encoder power states
#define ENCODER_PWR_OFF 0
#define ENCODER_PWR_ON 	1

void pwr_init(void);
bool pwr_check_battery(bool print);
inline void led_on(void);
inline void led_off(void);
inline void led_toggle(void);
void led_blink(void *p);
msg_t pwr_control_main(void *arg);

#endif