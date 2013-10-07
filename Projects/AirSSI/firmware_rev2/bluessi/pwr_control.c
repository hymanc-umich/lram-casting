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

#include "ch.h"
#include "hal.h"

#include "pwr_control.h"
#include "ssi.h"
#include "bluetooth.h"

#define ADC_GRP1_NUM_CHANNELS   1
#define ADC_GRP1_BUF_DEPTH      1

static adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH]; // ADC samples

volatile bool voltage_print_flag = FALSE;	// Voltage print to serial flag
volatile bool voltage_ready_flag = FALSE;	// Voltage read ready flag
volatile bool read_in_progress = FALSE;		// Read in progress flag
volatile uint16_t voltage = 4096;			// Voltage buffer
volatile bool battery_critical_flag = FALSE; // Battery critical voltage flag

VirtualTimer led_timer;						// LED Blink timer
VirtualTimer warning_timer;					// Low battery alert timer
/**
 * @brief ADC streaming callback.
 */
static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) 
{
  (void)adcp;
  (void) buffer;
  (void) n;
  if(adcp->state == ADC_COMPLETE)
  {
	voltage = samples[0];
	voltage_ready_flag = TRUE;
	read_in_progress = FALSE;
	if(voltage_print_flag)
	{
		voltage_print_flag = FALSE;
		bt_send_voltage(voltage);
	}
  }
}

/**
 * ADC conversion group.
 * Mode:        Linear buffer, 8 samples of 1 channel, SW triggered.
 * Channels:    IN10.
 */
static const ADCConversionGroup adcgrpcfg1 = 
{
  FALSE,
  ADC_GRP1_NUM_CHANNELS,
  adccallback,
  NULL,
  ADC_CFGR1_RES_12BIT,                              /* CFGRR1 */
  ADC_TR(0, 0),                                     /* TR */
  ADC_SMPR_SMP_28P5,                                 /* SMPR */
  ADC_CHSELR_CHSEL0                                /* CHSELR */
};

/**
 * @brief Power control system initialization
 */
void pwr_init(void)
{
  palSetPadMode(LED_PORT, LED_PIN, PAL_MODE_OUTPUT_PUSHPULL);  // Setup LED I/O
  palSetPadMode(BATTERY_ADC_PORT, BATTERY_ADC_PIN, PAL_MODE_INPUT_ANALOG);
  adcStart(&ADCD1, NULL);
}

/**
 * @breif Convenience function for turning LED on
 */
inline void led_on(void)
{
	palSetPad(LED_PORT, LED_PIN);
}

/** 
 * @brief Convenience function for turning the LED off
 */
inline void led_off(void)
{
	palClearPad(LED_PORT, LED_PIN);
}

/**
 * @brief Convenience function for toggling the LED
 */
inline void led_toggle(void)
{
	palTogglePad(LED_PORT, LED_PIN);
}


/**
 * @brief Callback function to shut LED off at end of blink
 */
static void led_blink_callback(void *p)
{
	palClearPad(LED_PORT, LED_PIN);
}

/**
 * @Brief Convenience function for a short 0.1s LED blink
 */
void led_blink(void *p)
{
	(void*) p;
	palSetPad(LED_PORT, LED_PIN);
	chSysLockFromIsr();
	chVTSetI(&led_timer, MS2ST(100), led_blink_callback, p);
	chSysUnlockFromIsr();
}

/**
 * @brief Callback for low battery alert timed event
 * @param void* p Parent 
 */
static void warning_timer_callback(void *p)
{
	(void *) p;
	led_toggle();
	uartStartSend(&UARTD1,30, "WARNING: BATTERY CRITICAL!!!\r\n");
	/*uint16_t i;
	for(i=0; i<1000;i++)
	{
		__asm__("NOP");
	}*/
	chSysLockFromIsr();
	chVTSetI(&warning_timer,MS2ST(2000), warning_timer_callback, p);
	chSysUnlockFromIsr();
}

/** 
 * @brief Starts ADC read of battery voltage
 * @param bool print Print voltage to serial flag
 * @return bool Successful start
 */
bool pwr_check_battery(bool print)
{
	if(!read_in_progress)
	{
		voltage_print_flag = print;
		voltage_ready_flag = FALSE;
		read_in_progress = TRUE;
		chSysLockFromIsr();
		adcStartConversionI(&ADCD1, &adcgrpcfg1, samples, ADC_GRP1_BUF_DEPTH);
		chSysUnlockFromIsr();
		return TRUE;
	}
	else if(print)
	{
		voltage_print_flag = TRUE;
	}
	return FALSE;
}

/**
 * @brief Power control thread main
 * @param void * arg Thread arguments (null)
 * @return msg_t Thread message stream
 */
msg_t pwr_control_main(void *arg)
{
	pwr_init();
	while(1)
	{
		chThdSleepMilliseconds(2000);
		pwr_check_battery(FALSE);// Read voltage and report
		//led_blink(&pwr_control_main);
		while(!voltage_ready_flag)
		{
			chThdSleepMicroseconds(100);
		}
		if(voltage <= BATT_MIN_THRESHOLD)
		{
			// TODO: Battery critical code
			if(!battery_critical_flag)
			{
				ssi_encoder_power(ENCODER_OFF);
				ssi_set_mode(SSI_MODE_POLL);
				battery_critical_flag = TRUE;
				warning_timer_callback(&pwr_control_main);
			}
		}
		else if(voltage <= BATT_LOW_THRESHOLD)
		{
			uartStartSend(&UARTD1,22,"WARNING: BATTERY LOW\r\n");
		}
		else
		{
			battery_critical_flag = FALSE;
		}
		
	}
}
