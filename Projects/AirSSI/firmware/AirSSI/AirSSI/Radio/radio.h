/*
 * radio.h
 *
 * Created: 5/30/2013 9:44:13 PM
 *  Author: Cody
 */ 


#ifndef RADIO_H_
#define RADIO_H_

// Radio Status Defines
#define STATUS_BATTERY_bm 1<<0
#define STATUS_ENCODER_POWER_bm 1<<1

struct radio
{
	uint8_t address;
	uint8_t channel;
	uint8_t status;
	
};
typedef struct radio radio_t;


void radio_initialize(uint8_t address);
void radio_set_channel(uint8_t channel);
void radio_send_position_packet(uint16_t position);
uint8_t radio_receive_bytes(void); // # of bytes received




#endif /* RADIO_H_ */