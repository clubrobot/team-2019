#ifndef __ARM_H__
#define __ARM_H__

#include <Arduino.h>
#include <math.h>
#include <HardwareSerial.h>
#include "../ShiftRegAX12.h"
#include "IK.h"

#define ARM_LEN_1 10.0
#define ARM_LEN_2 10.0
#define ARM_LEN_3 5.0

#define RX_AX12 16
#define TX_AX12 17

#define LATCHPIN	13
#define CLOCKPIN	12
#define DATAPIN		5 

#define AX12_DATA_CONTROL	0

extern ShiftRegAX12 servoax;
extern ShiftRegister shift;

class Arm
{
	public:

		Arm(double x, double y, double phi);

		void begin();
		void attach(unsigned char A1_id, unsigned char A2_id, unsigned char A3_id);
		void set_x(double x);
		void set_y(double y);
		void set_phi(double phi);
		void set_pos(double x, double y, double phi);
		joints_t reach_position(void);

		~Arm();

	private:

		IK *m_kinematics;
		

		coords_t m_tool;
		coords_t m_coord;
		joints_t m_joints;

		int m_A1_id;
		int m_A2_id;
		int m_A3_id;

};
#endif