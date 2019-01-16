#include "AX12.h"
#include "Dynamixel.h"	
#include <Arduino.h>

#define MIN(a,b) ((a)<(b)?(a):(b));
#define MAX(a,b) ((a)>(b)?(a):(b));

void AX12::SerialBegin(long baud, unsigned char control){
	Dynamixel.begin(baud, control);
}

void AX12::end(){
	Dynamixel.end();
}
	
void AX12::attach(unsigned char id){
	m_id = id;
}

void AX12::detach(){
	m_id = -1;
}

int AX12::reset()
{
	return Dynamixel.reset(m_id);
}

int AX12::ping(){
	return Dynamixel.ping(m_id);
}
	
int AX12::setID(unsigned char newID){
	return Dynamixel.setID(m_id, newID);
}
	
int AX12::setBD(long baud){
	return Dynamixel.setBD(m_id, baud);
}

int AX12::move(float Position){
	if(m_endlessMode){
		setEndlessMode(OFF);
	}
	int pos = MIN(1023,Position/300*1023);
	return Dynamixel.move(m_id, pos);
}

int AX12::moveSpeed(float Position, float speed){
	if(m_endlessMode){
		setEndlessMode(OFF);
	}
	int pos = MIN(1023,Position/300*1023);
	int sp  = MIN(1023,speed/114*1023);
	return Dynamixel.moveSpeed(m_id, pos, speed);
}

int AX12::setEndlessMode(bool Status){
	m_endlessMode = Status;
	return Dynamixel.setEndless(m_id, Status);
}
	
int AX12::turn(int Speed){
	if(Speed>0){
		int spd = MIN(1023, Speed);
		return Dynamixel.turn(m_id, 0, spd);
	}
	else{
		int spd = max(-1023, Speed);
		return Dynamixel.turn(m_id, 1, spd);
	}
}

int AX12::Nextmove(float Position){
	int pos = MIN(1023,Position/300*1023);
	return Dynamixel.moveRW(m_id, pos);
}
	
int AX12::NextmoveSpeed(float Position, float Speed){
	int pos = MIN(1023, Position/300*1023);
	return Dynamixel.moveSpeedRW(m_id, pos, Speed);
}
	
void AX12::action(){
	Dynamixel.setEndless(254, OFF);
	Dynamixel.action();
}

int AX12::setTempLimit(unsigned char Temperature){
	return Dynamixel.setTempLimit(m_id, Temperature);
}
	
int AX12::setAngleLimit(float CW, float CCW){
	int CWLimit = MIN(1023, CW/300*1023);
	int CCWLimit = MIN(1023, CCW/59 *1023);
	return Dynamixel.setAngleLimit(m_id, CWLimit, CCWLimit);
}

int AX12::setVoltageLimit(unsigned char DVoltage, unsigned char UVoltage){
	return Dynamixel.setVoltageLimit(m_id, DVoltage*10, UVoltage*10);
}

int AX12::setMaxTorque(int MaxTorque){
	return Dynamixel.setMaxTorque(m_id, MaxTorque);
}

int AX12::setMaxTorqueRAM(int MaxTorque){
	return Dynamixel.setMaxTorqueRAM(m_id, MaxTorque);
}

int AX12::setSRL(unsigned char SRL){
	return Dynamixel.setSRL(m_id, SRL);
}
	
int AX12::setRDT(unsigned char RDT){
	return Dynamixel.setRDT(m_id, RDT);
}
	
int AX12::setLEDAlarm(unsigned char LEDAlarm){
	return Dynamixel.setLEDAlarm(m_id, LEDAlarm);
}
	
int AX12::setShutdownAlarm(unsigned char SALARM){
	return Dynamixel.setShutdownAlarm(m_id, SALARM);
}

int AX12::setCMargin(unsigned char CWCMargin, unsigned char CCWCMargin){
	return Dynamixel.setCMargin(m_id, CWCMargin, CCWCMargin);
}
	
int AX12::setCSlope(unsigned char CWCSlope, unsigned char CCWCSlope){
	return Dynamixel.setCSlope(m_id, CWCSlope, CCWCSlope);
}
	
int AX12::setPunch(int Punch){
	return Dynamixel.setPunch(m_id, Punch);	
}

int AX12::moving(){
	return Dynamixel.moving(m_id);
}
	
int AX12::lockRegister(){
	return Dynamixel.lockRegister(m_id);
}
	
int AX12::savedMove(){
	return Dynamixel.RWStatus(m_id);
}	

int AX12::readTemperature(){
	return Dynamixel.readTemperature(m_id);
}
	
float AX12::readVoltage(){
	return Dynamixel.readVoltage(m_id)/10;
}
	
float AX12::readPosition(){
	float pos = (float)(Dynamixel.readPosition(m_id)) *300 /1023;
	return pos;
}

float AX12::readSpeed(){
	if(m_endlessMode){
		return Dynamixel.readSpeed(m_id);
	}
	else{
		return Dynamixel.readSpeed(m_id);// /1023*114 ;
	}
}

int AX12::readTorque(){
	int torque = Dynamixel.readLoad(m_id);
	if(torque <=1023){
		return map(torque, 0, 1023, 0, -1023);
	}
	else{
		return map(torque, 1024, 2047, 0, 1023);
	}
}
	
int AX12::hold(bool Status){
	m_holding = Status;
	return Dynamixel.torqueStatus(m_id, Status);
}

bool AX12::isHolding(){
	return m_holding;
}

int AX12::led(bool Status){
	return Dynamixel.ledStatus(m_id, Status);
}
