#ifndef __AX12_H
#define __AX12_H

#include "Dynamixel.h"	
#include <Arduino.h>

// mask error bit
#define INSTRUCTION_ERROR_MASK  	0x20
#define OVERLOAD_ERROR_MASK 		0x10
#define CHECKSUM_ERROR_MASK			0X08
#define	RANGE_ERROR_MASK			0x04
#define	ANGLE_LIMIT_ERROR_MASK 		0x02
#define	INPUT_VOLTAGE_ERROR_MASK 	0x01

// Macros to detect error type
#define INSTRUCTION_ERROR(x) 	(x & INSTRUCTION_ERROR_MASK)
#define OVERLOAD_ERROR(x) 		(x & OVERLOAD_ERROR_MASK)
#define CHECKSUM_ERROR(x) 		(x & CHECKSUM_ERROR_MASK)
#define	RANGE_ERROR(x) 			(x & RANGE_ERROR_MASK)
#define	ANGLE_LIMIT_ERROR(x) 	(x & ANGLE_LIMIT_ERROR_MASK)
#define	INPUT_VOLTAGE_ERROR(x)	(x & INPUT_VOLTAGE_ERROR_MASK)

class AX12 {

	private: 

		unsigned char m_id;
		bool m_endlessMode;
		bool m_holding;

	public:

		static void SerialBegin(long baud, unsigned char rx, unsigned char tx, unsigned char control);
		static void end();

		void attach(unsigned char id);
		void detach();

		int ping();
		
		int setID(unsigned char newID);
		int setBD(long baud);

		int move(float Position);
		int moveSpeed(float Position, float Speed);

		int setEndlessMode(bool Status);
		int turn(int Speed);
		
		int Nextmove(float Position);
		int NextmoveSpeed(float Position, float Speed);
		
		void static action();

		int setTempLimit(unsigned char Temperature);
		int setAngleLimit(float CWLimit, float CCWLimit);
		int setVoltageLimit(unsigned char DVoltage, unsigned char UVoltage);
		int setMaxTorque(int MaxTorque);
		int setMaxTorqueRAM(int MaxTorque);
		int setSRL(unsigned char SRL);
		int setRDT(unsigned char RDT);
		int setLEDAlarm(unsigned char LEDAlarm);
		int setShutdownAlarm(unsigned char SALARM);
		int setCMargin(unsigned char CWCMargin, unsigned char CCWCMargin);
		int setCSlope( unsigned char CWCSlope, unsigned char CCWCSlope);
		int setPunch(int Punch);

		int moving();
		int lockRegister();
		int savedMove();
		
		int readTemperature();
		float readVoltage();
		float readPosition();
		float readSpeed();
		int readTorque();

		bool isHolding();
		
		int hold(bool Status);
		int led(bool Status);
	
};
#endif /* __AX12_H */