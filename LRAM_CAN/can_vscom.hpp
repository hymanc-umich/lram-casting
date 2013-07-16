#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vs_can_api.h>

class Can_Vscom
{
	
	public:
		write(uint8_t node_id, char * write_cmd);
		read(uint8_t node_id, char * read_cmd);
	private:
}