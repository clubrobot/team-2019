#include <Arduino.h>
#include <Servo.h>
#include "RobotArm.h"
#include "ShiftRegAX12.h" 
#include "SoftwareSerial.h"
#include "StepByStepMotor.h"

extern ShiftRegAX12 servoax;
extern StepByStepMotor motor;

RobotArm::RobotArm(double x, double y, double z, double theta, float speed)
{
	m_x = x;
	m_y = y;
	m_z = z * 10;
	m_theta = theta;

	m_speed = speed;
}

void RobotArm::attach(unsigned char A1_id, unsigned char A2_id, unsigned char A3_id, unsigned char servo)
{
	m_A1_id = A1_id;
	m_A2_id = A2_id;
	m_A3_id = A3_id;

	m_gripper.attach(servo);
}

void RobotArm::begin()
{
	motor.begin();
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

	//Set start position
	ReachPosition(m_x, m_y, m_z, m_theta, Z_FIRST);
}

bool RobotArm::solve_angles(double x, double y)
{
	double D1,D2,R2;
	//First, get the length of line dist.	
	double dist = distance(x, y);
	//Calculating angle D1 is trivial. Atan2 is a modified arctan() function that returns unambiguous results.	
	D1 = atan2(y, x);
	//D2 can be calculated using the law of cosines where a = dist, b = len1, and c = len2.	
	if(!lawOfCosines(dist, ARM_LEN_1, ARM_LEN_2, &D2)) return false;
	//Then A1 is simply the sum of D1 and D2.	
	

	if(!lawOfCosines(ARM_LEN_1, ARM_LEN_2, dist, &R2)) return false;
	//A2 can also be calculated with the law of cosine, but this time with a = len1, b = len2, and c = dist.	
	if(x >= 0)
	{
		m_A1 = convert_deg(D1 + D2);
		m_A2 = convert_deg(R2);
	}
	else
	{
		m_A1 = convert_deg(D1 - D2);
		m_A2 = 360 - convert_deg(R2);
	}

	return true;
}

bool RobotArm::solve_coords(double x, double y)
{
	if((x >= X_MIN_UP) && (x<= X_MAX_UP))
	{
		if(y <= up(x) && y >= down(x))
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
    
    return false;
}

bool RobotArm::is_reached()
{
	bool ret1,ret2;
	double A1,A2;

	A1 = get_A1();
	A2 = get_A2();

	if(abs(A1 - get_A1theo()) <= DELTA_POS)
		ret1 = true;
	else
		ret1 = false;

	if(abs(A2 - get_A2theo()) <= DELTA_POS)
		ret2 = true;
	else
		ret2 = false;

	return ret1 & ret2;

}

bool RobotArm::ReachPosition(double x, double y, double z, double theta, int z_order)
{
	m_x 	= x;
	m_y 	= y;
	m_z 	= z;

	m_theta = theta;

	double t1,t2,t3;

	if(solve_angles(x,y))
	{

		if(z_order == Z_FIRST)
			motor.set_position(m_z);


		//calculate real position with offset
		t1 = m_A1 + ARM1_OFFSET;

		t2 = m_A2 - ARM2_OFFSET;

		// send pos to AX12 servos
		servoax.attach(m_A1_id);
		servoax.setMaxTorqueRAM(1023);
		servoax.moveSpeed((float)t1, m_speed);
		servoax.detach();

		servoax.attach(m_A2_id);
		servoax.setMaxTorqueRAM(1023);
		servoax.moveSpeed((float)t2, m_speed);
		servoax.detach();

		
		m_A3 = (360-(m_A1+m_A2)) + m_theta;
		t3 = m_A3 + ARM3_OFFSET;

		servoax.attach(m_A3_id);
		servoax.setMaxTorqueRAM(1023);
		servoax.moveSpeed((float)t3, m_speed);
		servoax.detach();


		if(z_order == Z_LAST)
			motor.set_position(m_z);

		return true;
	}
	else
	{
		return false;
	}

}

void RobotArm::set_angles(float A1, float A2, float A3)
{
	// send pos to AX12 servos
	servoax.attach(m_A1_id);
	servoax.moveSpeed((float)A1, m_speed);

	servoax.attach(m_A2_id);
	servoax.moveSpeed((float)A2, m_speed);

	servoax.attach(m_A3_id);
	servoax.moveSpeed((float)A3, m_speed);
}


bool RobotArm::set_x(double x)
{
	m_x = x;
	return ReachPosition(m_x, m_y, m_z,m_theta, Z_FIRST);
}

bool RobotArm::set_y(double y)
{
	m_y = y;
	return ReachPosition(m_x, m_y, m_z, m_theta, Z_FIRST);
}

bool RobotArm::set_z(double z)
{
	m_z = z*10;
	motor.set_position(m_z);	
	return true;
}

bool RobotArm::set_theta(double theta)
{
	m_theta = theta;
			//calculate gripper angle classic
	m_A3 = (360-(m_A1+m_A2)) + m_theta;
	double t = m_A3 + ARM3_OFFSET;

	servoax.attach(m_A3_id);
	servoax.setMaxTorqueRAM(1023);
	servoax.moveSpeed((float)t, m_speed);
	servoax.detach();
	return true;
}

void RobotArm::set_speed(float speed)
{
	m_speed = speed;
}

void RobotArm::close_gripper()
{
	m_gripper.write(GRIPPER_CLOSE);
}

void RobotArm::open_gripper()
{
	m_gripper.write(GRIPPER_OPEN);
}

double RobotArm::get_A1theo()
{
	return m_A1 + ARM1_OFFSET;
}

double RobotArm::get_A2theo()
{
	return m_A2 - ARM2_OFFSET;
}

double RobotArm::get_A3theo()
{
	return m_A3 + ARM3_OFFSET;
}

double RobotArm::get_A1()
{
	servoax.attach(m_A1_id);
	return servoax.readPosition();
}

double RobotArm::get_A2()
{
	servoax.attach(m_A2_id);
	return servoax.readPosition();
}

double RobotArm::get_A3()
{
	servoax.attach(m_A3_id);
	return servoax.readPosition();
}

bool RobotArm::lawOfCosines(double a, double b, double c, double *A)
{
	double den,cc;
	den = 2*a*b;

	if(den == 0) return false;

	cc =(a*a + b*b - c*c) / den;

	if(cc>1 || cc<-1) return false;

	*A = acos(cc);

	return true;
}

double RobotArm::distance(double x, double y)
{
	return sqrt(x*x + y*y);
}

double RobotArm::convert_deg(double rad)
{
	return rad * 180 / M_PI;
}

double RobotArm::up(double x)
{
	return sqrt(X_MAX_UP * X_MAX_UP - x * x);
}

double RobotArm::down(double x)
{
	if((x >= X_MIN_UP) && (x <= X_MIN_DOWN))
	{
		return -sqrt( (RAY_1 * RAY_1) - ((x - X_MIN_DOWN) * (x - X_MIN_DOWN)));
	}
	else if((x > X_MIN_DOWN) && (x < X_MAX_DOWN))
	{
		return sqrt( (RAY_2 * RAY_2) - (x * x));
	}
	else if((x <= X_MAX_UP) && (x >= X_MAX_DOWN))
	{
		return -sqrt( (RAY_3 * RAY_3) - ((x - X_MAX_DOWN) * (x - X_MAX_DOWN)));
	}
	return -1;
}