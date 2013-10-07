/*  bluetooth.h AirSSI Module Bluetooth Communication Manager
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

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "ch.h"
#include "hal.h"

#define USART1_PORT GPIOA
#define USART1_TX_PIN 9
#define USART1_RX_PIN 10

#define BT_CMD_REQUEST_POSITION 'r'
#define BT_CMD_EXIT_STREAM 'e'
#define BT_CMD_START_STREAM 's'
#define BT_CMD_INCR_STREAM_DELAY '+'
#define BT_CMD_DECR_STREAM_DELAY '-'
#define BT_CMD_ENABLE_ENC_PWR 'p'
#define BT_CMD_DISABLE_ENC_PWR 'd'
#define BT_CMD_GET_BATT_VOLT 'v'
#define BT_CMD_CHANGE_FORMAT 'f'
#define BT_CMD_HELP 'h'

#define RX_BUFFER_SIZE 32

void bt_init(UARTDriver * uartp);
bool bt_send_voltage(uint16_t voltage);
bool bt_send_position(uint32_t pos);
bool bt_get_tx_state(void);
#endif