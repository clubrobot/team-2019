#include <Arduino.h>
#include "Arm.h"
#include <HardwareSerial.h>
#include "../ShiftRegAX12.h"


Arm::Arm(double x, double y, double phi)
{
	m_coord.x 	= x;
	m_coord.y	= y;
	m_coord.phi	= phi;

	m_kinematics = new Picker(ARM_LEN_1, ARM_LEN_2 ,ARM_LEN_3, m_joints, m_coord);

	servoax.SerialBegin(9600, RX_AX12, TX_AX12, AX12_DATA_CONTROL);

	shift.attach(LATCHPIN,CLOCKPIN,DATAPIN);
}
Arm::~Arm(){}

void Arm::begin()
{
	//Set AX configx
	//Broadcast address
	servoax.attach(254);

	//servoax.setSRL(1); // Respond only to READ_DATA instructions
	servoax.setLEDAlarm(32); // max torque only

	servoax.setShutdownAlarm(32); // max torque only
	servoax.setMaxTorque(1023);

	servoax.setCMargin(0X00,0X00); //Compliance margin
	servoax.setCSlope(0X60,0X60); //compliance slope

	servoax.setEndlessMode(OFF);
	servoax.hold(OFF);
}

void Arm::attach(unsigned char A1_id, unsigned char A2_id, unsigned char A3_id)
{
	m_A1_id = A1_id;
	m_A2_id = A2_id;
	m_A3_id = A3_id;
}

void Arm::set_x(double x)
{
	m_tool   = m_kinematics->forward_kinematics(m_joints);
	m_tool.x = x;
}

void Arm::set_y(double y)
{
	m_tool 	 = m_kinematics->forward_kinematics(m_joints);
	m_tool.y = y;
}

void Arm::set_phi(double phi)
{
	m_tool 		= m_kinematics->forward_kinematics(m_joints);
	m_tool.phi  = phi;
}

void Arm::set_pos(double x, double y, double phi)
{
	m_tool 		= m_kinematics->forward_kinematics(m_joints);
	m_tool.x 	= x;
	m_tool.y 	= y;
	m_tool.phi  = phi;
}

joints_t Arm::reach_position(void)
{
	m_joints = m_kinematics->inverse_kinematics(m_tool);

	// send pos to AX12 servos
	servoax.attach(m_A1_id);
	servoax.setMaxTorqueRAM(1023);
	servoax.move((float)map(m_joints.th1, -M_PI, M_PI, 0, 1023));
	servoax.detach();

	servoax.attach(m_A2_id);
	servoax.setMaxTorqueRAM(1023);
	servoax.move((float)map(m_joints.th3, -M_PI, M_PI, 0, 1023));
	servoax.detach();


	servoax.attach(m_A3_id);
	servoax.setMaxTorqueRAM(1023);
	servoax.move((float)map(m_joints.th3, -M_PI, M_PI, 0, 1023));
	servoax.detach();

	return m_joints;
}