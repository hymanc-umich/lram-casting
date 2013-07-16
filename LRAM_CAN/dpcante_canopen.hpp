#ifndef DPCANTE_CANOPEN
#define DPCANTE_CANOPEN

#include <string>
#include <iostream>
#include <errstream>

// RPDO COB-ID Base Values
#define RPDO1_COB_BASE 0x201
#define RPDO2_COB_BASE 0x301
#define RPDO3_COB_BASE 0x401
#define RPDO4_COB_BASE 0x411
#define RPDO5_COB_BASE 0x421
#define RPDO21_COB_BASE 0x501
#define RPDO22_COB_BASE 0x511
#define RPDO23_COB_BASE 0x521
#define RPDO24_COB_BASE 0x531

// PDO Object Index Values
#define INDEX_CONTROLWORD 0x6040
#define INDEX_MODE_OF_OPERATION 0x6060
#define INDEX_OPERATION_DISPLAY 0x6061
#define INDEX_COMMAND_LIMITER_PARAMETERS 0x20C3
#define INDEX_MOTION_PROFILE_TYPE 0x6086
#define INDEX_VELOCITY_LIMITS 0x2037
#define INDEX_VELOCITYt_LOOP_CONTROL_PARAMETERS 0x2036
#define INDEX_VELOCITY_VALUES 0x2011
#define INDEX_VELOCITY_SENSOR_ACTUAL_VALUES 0x6069
#define INDEX_VELOCITY_DEMAND 0x606B
#define INDEX_ACTUAL_VELOCITY 0x606C
#define INDEX_TARGET_VELOCITY 0x60FF

#define CONTROLWORD_RESET_FAULT 0x80
#define CONTROLWORD_DISABLE_VOLTAGE 0x04
#define CONTROLWORD_SHUTDOWN
#define CONTROLWORD_SWITCH_ON
#define CONTROLWORD_ENABLE_OPERATION
#define CONTROLWORD_QUICK_STOP
#define CONTROLWORD_BEGIN_HOMING
#define CONTROLWORD_END_HOMING
/* AMC Amplifier Modes */
enum amplifier_mode{
					PROFILE_POSITION, 
					PROFILE_VELOCITY, 
					PROFILE_TORQUE, 
					HOMING,
					INTERPOLATED_POSITION,
					CYCLIC_SYNC_POSITION,
					CYCLIC_SYNC_VELOCITY,
					CYCLIC_SYNC_TORQUE,
					CUSTOM
					};
					
typedef enum amplifier_mode amp_mode_t;

class DPCANTE
{
	public:
		DPCANTE(Canopen_node *can);
		~DPCANTE(void);
		unsigned char get_node_id(void);
		bool get_bridge_state(void);
		bool enable_bridge(void);
		bool disable_bridge(void);
		bool set_amplifier_mode(amp_mode_t);
		bool get_amplifier_mode(void);
		
		/* TPDO Calls */
		bool set_velocity(int velocity);
		bool set_position(int position);
		bool set_torque(int torque);
		
		
	private:
		unsigned char node_id;
		bool bridge_state;
		amp_mode_t amp_mode;
		Canopen_node *can;
};

#endif