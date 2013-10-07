/*  main.c AirSSI Module Firmware v2.0
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

/* This program is dependent upon ChibiOS 2.6.1 Please see the ChibiOS
   Licensing Note
*/

/* ChibiOS Licensing Note
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "ssi.h"
#include "bluetooth.h"
#include "pwr_control.h"

static WORKING_AREA(ssi_working_area, 512); // SSI thread static allocation
static WORKING_AREA(pwr_ctl_working_area, 128); // Power ADC thread static allocation
 
/**
 * @brief Root thread main
 */
int main(void) {
	halInit();		// Initialize ChibiOS HAL drivers
	chSysInit();	// Initialize ChibiOS kernel
	
	bt_init(&UARTD1);
	// Start threads
	Thread *ssi_thread = chThdCreateStatic(ssi_working_area, sizeof(ssi_working_area), HIGHPRIO, ssi_main, NULL);
	Thread *pwr_ctl_thread = chThdCreateStatic(pwr_ctl_working_area, sizeof(pwr_ctl_working_area), LOWPRIO, pwr_control_main, NULL);

	// Main loop
	while (1) 
	{
		chThdSleepMilliseconds(100);	// Idle perpetually while background threads run
	}
}
