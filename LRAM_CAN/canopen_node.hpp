#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/sockets.h>

#include <net/if.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

/* Simple CANOpen Node Class */
class Canopen_Node
{
	unsigned char id; // CANOpen Node ID (1-127
	string interface;
	struct sockaddr_can address;
	struct ifreq ifr;
	int socket;
	
	public:
		int get_id(void);
	private:
		void send_frame(
} conode;