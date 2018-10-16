#include "ShiftRegAX12.h"	
#include <Arduino.h>
#include "SoftwareSerial.h"
#include "../common/SerialTalks.h"
#include "../common/ShiftRegister.h"

extern SoftwareSerial SoftSerial;
extern ShiftRegister shift;
// Macro for the selection of the Serial Port

#define sendData(args)  (SoftSerial.write(args))    // Write Over Serial
#define availableData() (SoftSerial.available())    // Check Serial Data Available
#define readData()      (SoftSerial.read())         // Read Serial Data
#define peekData()      ((unsigned byte) SoftSerial.peek())         // Peek Serial Data
#define beginCom(args)  (SoftSerial.begin(args))    // Begin Serial Comunication
#define endCom()        (SoftSerial.end())          // End Serial Comunication

#define setRXPin(args)  (SoftSerial.setRX(args))    // Set Rx Serial Pin
#define setTXPin(args)  (SoftSerial.setTX(args))    // Set Tx Serial Pin

// Macro for Timing

#define delayus(args) (delayMicroseconds(args))  // Delay Microseconds

// Macro for Comunication Flow Control

#define setDPin(DirPin,Mode)   (pinMode(DirPin,Mode))       // Select the Switch to TX/RX Mode Pin


// Private Methods //////////////////////////////////////////////////////////////

int ShiftDynamixelClass::read_error(void)
{
	Time_Counter = 0;
	while((availableData() < 5) & (Time_Counter < TIME_OUT)){  // Wait for Data
		Time_Counter++;
		delayus(1000);
	}
	while (availableData() > 0){
		Incoming_Byte = readData();
		if ( (Incoming_Byte == 255) & (peekData() == 255) ){
			readData();                                    // Start Bytes
			readData();                                    // Ax-12 ID
			readData();                                    // Length
			Error_Byte = readData();                       // Error
				return (Error_Byte);
		}
	}
	return (-1);											 // No Ax Response
}

// Public Methods //////////////////////////////////////////////////////////////

void ShiftDynamixelClass::begin(long baud,unsigned char Rx, unsigned char Tx)
{	
	
	DTx = Tx;
	DRx = Rx;
	setRXPin(DRx);
	setTXPin(DTx);
	beginCom(baud);
	
}

void ShiftDynamixelClass::begin(long baud,unsigned char Rx, unsigned char Tx, unsigned char D_Pin)
{	
		DTx = Tx;
		DRx = Rx;
		setRXPin(DRx);
		setTXPin(DTx);
		beginCom(baud);
		Direction_Pin = D_Pin;
}


void ShiftDynamixelClass::end()
{
	endCom();
}

int ShiftDynamixelClass::reset(unsigned char ID)
{
	Checksum = (~(ID + AX_RESET_LENGTH + AX_RESET))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_RESET_LENGTH);
	sendData(AX_RESET);    
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);

    return (read_error());  
}

int ShiftDynamixelClass::ping(unsigned char ID)
{
	Checksum = (~(ID + AX_READ_DATA + AX_PING))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_READ_DATA);
	sendData(AX_PING);    
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());              
}

int ShiftDynamixelClass::setID(unsigned char ID, unsigned char newID)
{    
	Checksum = (~(ID + AX_ID_LENGTH + AX_WRITE_DATA + AX_ID + newID))&0xFF;

	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_ID_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_ID);
    sendData(newID);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int ShiftDynamixelClass::setBD(unsigned char ID, long baud)
{    
	unsigned char Baud_Rate = (2000000/baud) - 1;
    Checksum = (~(ID + AX_BD_LENGTH + AX_WRITE_DATA + AX_BAUD_RATE + Baud_Rate))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                 // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_BD_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_BAUD_RATE);
    sendData(Baud_Rate);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int ShiftDynamixelClass::move(unsigned char ID, int Position)
{
    char Position_H,Position_L;
    Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
    Position_L = Position;
	Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;
    
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                 // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_GOAL_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_GOAL_POSITION_L);
    sendData(Position_L);
    sendData(Position_H);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);

    return (read_error());                 // Return the read error
}

int ShiftDynamixelClass::moveSpeed(unsigned char ID, int Position, int Speed)
{
    char Position_H,Position_L,Speed_H,Speed_L;
    Position_H = Position >> 8;    
    Position_L = Position;                // 16 bits - 2 x 8 bits variables
    Speed_H = Speed >> 8;
    Speed_L = Speed;                      // 16 bits - 2 x 8 bits variables
	Checksum = (~(ID + AX_GOAL_SP_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H + Speed_L + Speed_H))&0xFF;
 
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_GOAL_SP_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_GOAL_POSITION_L);
    sendData(Position_L);
    sendData(Position_H);
    sendData(Speed_L);
    sendData(Speed_H);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());               // Return the read error
}

int ShiftDynamixelClass::setEndless(unsigned char ID, bool Status)
{
 if ( Status ) {	
	  char AX_CCW_AL_LT = 0;     // Changing the CCW Angle Limits for Full Rotation.
	  Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L))&0xFF;
	
	  shift.write(Direction_Pin,Tx_MODE);
      sendData(AX_START);                // Send Instructions over Serial
      sendData(AX_START);
      sendData(ID);
      sendData(AX_GOAL_LENGTH);
      sendData(AX_WRITE_DATA);
      sendData(AX_CCW_ANGLE_LIMIT_L );
      sendData(AX_CCW_AL_LT);
      sendData(AX_CCW_AL_LT);
      sendData(Checksum);
      delayus(TX_DELAY_TIME);
	  shift.write(Direction_Pin,Rx_MODE);

	  return(read_error());
 }
 else
 {
	 turn(ID,0,0);
	 Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L + AX_CCW_AL_L + AX_CCW_AL_H))&0xFF;
	
	 shift.write(Direction_Pin,Tx_MODE);
	 sendData(AX_START);                 // Send Instructions over Serial
	 sendData(AX_START);
	 sendData(ID);
	 sendData(AX_GOAL_LENGTH);
	 sendData(AX_WRITE_DATA);
	 sendData(AX_CCW_ANGLE_LIMIT_L);
	 sendData(AX_CCW_AL_L);
	 sendData(AX_CCW_AL_H);
	 sendData(Checksum);
	 delayus(TX_DELAY_TIME);
	 shift.write(Direction_Pin,Rx_MODE);
	 
	 return (read_error());                 // Return the read error
  }
 } 

int ShiftDynamixelClass::turn(unsigned char ID, bool SIDE, int Speed)
{		
		if (SIDE == 0){                          // Move Left///////////////////////////
			
			char Speed_H,Speed_L;
			Speed_H = Speed >> 8;
			Speed_L = Speed;                     // 16 bits - 2 x 8 bits variables
			Checksum = (~(ID + AX_SPEED_LENGTH + AX_WRITE_DATA + AX_GOAL_SPEED_L + Speed_L + Speed_H))&0xFF;
			
			shift.write(Direction_Pin,Tx_MODE);
			sendData(AX_START);                // Send Instructions over Serial
			sendData(AX_START);
			sendData(ID);
			sendData(AX_SPEED_LENGTH);
			sendData(AX_WRITE_DATA);
			sendData(AX_GOAL_SPEED_L);
			sendData(Speed_L);
			sendData(Speed_H);
			sendData(Checksum);
			delayus(TX_DELAY_TIME);
			shift.write(Direction_Pin,Rx_MODE);
			
			return(read_error());               // Return the read error		
		}
		else
		{                                            // Move Rigth////////////////////
			char Speed_H,Speed_L;
			Speed_H = (Speed >> 8) + 4;
			Speed_L = Speed;                     // 16 bits - 2 x 8 bits variables
			Checksum = (~(ID + AX_SPEED_LENGTH + AX_WRITE_DATA + AX_GOAL_SPEED_L + Speed_L + Speed_H))&0xFF;
			
			shift.write(Direction_Pin,Tx_MODE);
			sendData(AX_START);                // Send Instructions over Serial
			sendData(AX_START);
			sendData(ID);
			sendData(AX_SPEED_LENGTH);
			sendData(AX_WRITE_DATA);
			sendData(AX_GOAL_SPEED_L);
			sendData(Speed_L);
			sendData(Speed_H);
			sendData(Checksum);
			delayus(TX_DELAY_TIME);
			shift.write(Direction_Pin,Rx_MODE);
			
			return(read_error());                // Return the read error	
		}
}

int ShiftDynamixelClass::moveRW(unsigned char ID, int Position)
{
    char Position_H,Position_L;
    Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
    Position_L = Position;
    Checksum = (~(ID + AX_GOAL_LENGTH + AX_REG_WRITE + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;

	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                 // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_GOAL_LENGTH);
    sendData(AX_REG_WRITE);
    sendData(AX_GOAL_POSITION_L);
    sendData(Position_L);
    sendData(Position_H);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    return (read_error());                 // Return the read error
}

int ShiftDynamixelClass::moveSpeedRW(unsigned char ID, int Position, int Speed)
{
    char Position_H,Position_L,Speed_H,Speed_L;
    Position_H = Position >> 8;    
    Position_L = Position;                // 16 bits - 2 x 8 bits variables
    Speed_H = Speed >> 8;
    Speed_L = Speed;                      // 16 bits - 2 x 8 bits variables
    Checksum = (~(ID + AX_GOAL_SP_LENGTH + AX_REG_WRITE + AX_GOAL_POSITION_L + Position_L + Position_H + Speed_L + Speed_H))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_GOAL_SP_LENGTH);
    sendData(AX_REG_WRITE);
    sendData(AX_GOAL_POSITION_L);
    sendData(Position_L);
    sendData(Position_H);
    sendData(Speed_L);
    sendData(Speed_H);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());               // Return the read error
}

void ShiftDynamixelClass::action()
{	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(BROADCAST_ID);
    sendData(AX_ACTION_LENGTH);
    sendData(AX_ACTION);
    sendData(AX_ACTION_CHECKSUM);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
}

int ShiftDynamixelClass::torqueStatus( unsigned char ID, bool Status)
{
    Checksum = (~(ID + AX_TORQUE_LENGTH + AX_WRITE_DATA + AX_TORQUE_ENABLE + Status))&0xFF;

	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);              // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_TORQUE_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_TORQUE_ENABLE);
    sendData(Status);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());              // Return the read error
}

int ShiftDynamixelClass::ledStatus(unsigned char ID, bool Status)
{    
    Checksum = (~(ID + AX_LED_LENGTH + AX_WRITE_DATA + AX_LED + Status))&0xFF;

	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);              // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_LED_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_LED);
    sendData(Status);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());              // Return the read error
}

int ShiftDynamixelClass::readTemperature(unsigned char ID)
{	
    Checksum = (~(ID + AX_TEM_LENGTH  + AX_READ_DATA + AX_PRESENT_TEMPERATURE + AX_BYTE_READ))&0xFF;
    
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_TEM_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_TEMPERATURE);
    sendData(AX_BYTE_READ);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    Temperature_Byte = -1;
    Time_Counter = 0;
    while((availableData() < 6) & (Time_Counter < TIME_OUT)){
		Time_Counter++;
		delayus(1000);
    }
	
    while (availableData() > 0){
		Incoming_Byte = readData();
		if ( (Incoming_Byte == 255) & (peekData() == 255) ){
			readData();                            // Start Bytes
			readData();                            // Ax-12 ID
			readData();                            // Length
			if( (Error_Byte = readData()) != 0 )   // Error
				return (Error_Byte*(-1));
			Temperature_Byte = readData();         // Temperature
		}
    }
	return (Temperature_Byte);               // Returns the read temperature
}

int ShiftDynamixelClass::readPosition(unsigned char ID)
{	
    Checksum = (~(ID + AX_POS_LENGTH  + AX_READ_DATA + AX_PRESENT_POSITION_L + AX_BYTE_READ_POS))&0xFF;
  
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_POS_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_POSITION_L);
    sendData(AX_BYTE_READ_POS);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    Position_Long_Byte = -1;
	Time_Counter = 0;
    while((availableData() < 7) & (Time_Counter < TIME_OUT)){
		Time_Counter++;
		delayus(1000);
    }
	
    while (availableData() > 0){
		Incoming_Byte = readData();
		if ( (Incoming_Byte == 255) & (peekData() == 255) ){
			readData();                            // Start Bytes
			readData();                            // Ax-12 ID
			readData();                            // Length
			if( (Error_Byte = readData()) != 0 )   // Error
				return (Error_Byte*(-1));
    
			Position_Low_Byte = readData();            // Position Bytes
			Position_High_Byte = readData();
			Position_Long_Byte = Position_High_Byte << 8; 
			Position_Long_Byte = Position_Long_Byte + Position_Low_Byte;
		}
    }
	return (Position_Long_Byte);     // Returns the read position
}

int ShiftDynamixelClass::readVoltage(unsigned char ID)
{    
    Checksum = (~(ID + AX_VOLT_LENGTH  + AX_READ_DATA + AX_PRESENT_VOLTAGE + AX_BYTE_READ))&0xFF;
    
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_VOLT_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_VOLTAGE);
    sendData(AX_BYTE_READ);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    Voltage_Byte = -1;
	Time_Counter = 0;
    while((availableData() < 6) & (Time_Counter < TIME_OUT)){
		Time_Counter++;
		delayus(1000);
    }
	
    while (availableData() > 0){
		Incoming_Byte = readData();
		if ( (Incoming_Byte == 255) & (peekData() == 255) ){
			readData();                            // Start Bytes
			readData();                            // Ax-12 ID
			readData();                            // Length
			if( (Error_Byte = readData()) != 0 )   // Error
				return (Error_Byte*(-1));
			Voltage_Byte = readData();             // Voltage
		}
    }
	return (Voltage_Byte);               // Returns the read Voltage
}

int ShiftDynamixelClass::setTempLimit(unsigned char ID, unsigned char Temperature)
{
	Checksum = (~(ID + AX_TL_LENGTH +AX_WRITE_DATA+ AX_LIMIT_TEMPERATURE + Temperature))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_TL_LENGTH);
	sendData(AX_WRITE_DATA);
	sendData(AX_LIMIT_TEMPERATURE);
    sendData(Temperature);
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int ShiftDynamixelClass::setVoltageLimit(unsigned char ID, unsigned char DVoltage, unsigned char UVoltage)
{
	Checksum = (~(ID + AX_VL_LENGTH +AX_WRITE_DATA+ AX_DOWN_LIMIT_VOLTAGE + DVoltage + UVoltage))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_VL_LENGTH);
	sendData(AX_WRITE_DATA);
	sendData(AX_DOWN_LIMIT_VOLTAGE);
    sendData(DVoltage);
    sendData(UVoltage);
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int ShiftDynamixelClass::setAngleLimit(unsigned char ID, int CWLimit, int CCWLimit)
{
	char CW_H,CW_L,CCW_H,CCW_L;
    CW_H = CWLimit >> 8;    
    CW_L = CWLimit;                // 16 bits - 2 x 8 bits variables
    CCW_H = CCWLimit >> 8;
    CCW_L = CCWLimit;  
	Checksum = (~(ID + AX_VL_LENGTH +AX_WRITE_DATA+ AX_CW_ANGLE_LIMIT_L + CW_H + CW_L + AX_CCW_ANGLE_LIMIT_L + CCW_H + CCW_L))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_CCW_CW_LENGTH);
	sendData(AX_WRITE_DATA);
	sendData(AX_CW_ANGLE_LIMIT_L);
    sendData(CW_L);
	sendData(CW_H);
	sendData(AX_CCW_ANGLE_LIMIT_L);
    sendData(CCW_L);
	sendData(CCW_H);
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int ShiftDynamixelClass::setMaxTorque(unsigned char ID, int MaxTorque)
{
    char MaxTorque_H,MaxTorque_L;
    MaxTorque_H = MaxTorque >> 8;           // 16 bits - 2 x 8 bits variables
    MaxTorque_L = MaxTorque;
	Checksum = (~(ID + AX_MT_LENGTH + AX_WRITE_DATA + AX_MAX_TORQUE_L + MaxTorque_L + MaxTorque_H))&0xFF;
    
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                 // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_MT_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_MAX_TORQUE_L);
    sendData(MaxTorque_L);
    sendData(MaxTorque_H);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    return (read_error());                 // Return the read error
}

int ShiftDynamixelClass::setMaxTorqueRAM(unsigned char ID, int MaxTorque)
{
    char MaxTorque_H,MaxTorque_L;
    MaxTorque_H = MaxTorque >> 8;           // 16 bits - 2 x 8 bits variables
    MaxTorque_L = MaxTorque;
	Checksum = (~(ID + AX_MT_LENGTH + AX_WRITE_DATA + 0x22 + MaxTorque_L + MaxTorque_H))&0xFF;
    
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                 // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_MT_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(0x22);
    sendData(MaxTorque_L);
    sendData(MaxTorque_H);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    return (read_error());                 // Return the read error
}

int ShiftDynamixelClass::setSRL(unsigned char ID, unsigned char SRL)
{    
	Checksum = (~(ID + AX_SRL_LENGTH + AX_WRITE_DATA + AX_RETURN_LEVEL + SRL))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_SRL_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_RETURN_LEVEL);
    sendData(SRL);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int ShiftDynamixelClass::setRDT(unsigned char ID, unsigned char RDT)
{    
	Checksum = (~(ID + AX_RDT_LENGTH + AX_WRITE_DATA + AX_RETURN_DELAY_TIME + (RDT/2)))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_RDT_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_RETURN_DELAY_TIME);
    sendData((RDT/2));
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int ShiftDynamixelClass::setLEDAlarm(unsigned char ID, unsigned char LEDAlarm)
{    
	Checksum = (~(ID + AX_LEDALARM_LENGTH + AX_WRITE_DATA + AX_ALARM_LED + LEDAlarm))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_LEDALARM_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_ALARM_LED);
    sendData(LEDAlarm);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int ShiftDynamixelClass::setShutdownAlarm(unsigned char ID, unsigned char SALARM)
{    
	Checksum = (~(ID + AX_SALARM_LENGTH + AX_WRITE_DATA + AX_ALARM_SHUTDOWN + SALARM))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_SALARM_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_ALARM_SHUTDOWN);
    sendData(SALARM);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int ShiftDynamixelClass::setCMargin(unsigned char ID, unsigned char CWCMargin, unsigned char CCWCMargin)
{
	Checksum = (~(ID + AX_CM_LENGTH +AX_WRITE_DATA+ AX_CW_COMPLIANCE_MARGIN + CWCMargin + AX_CCW_COMPLIANCE_MARGIN + CCWCMargin))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_CM_LENGTH);
	sendData(AX_WRITE_DATA);
	sendData(AX_CW_COMPLIANCE_MARGIN);
    sendData(CWCMargin);
	sendData(AX_CCW_COMPLIANCE_MARGIN);
    sendData(CCWCMargin);
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int ShiftDynamixelClass::setCSlope(unsigned char ID, unsigned char CWCSlope, unsigned char CCWCSlope)
{
	Checksum = (~(ID + AX_CS_LENGTH +AX_WRITE_DATA+ AX_CW_COMPLIANCE_SLOPE + CWCSlope + AX_CCW_COMPLIANCE_SLOPE + CCWCSlope))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_CS_LENGTH);
	sendData(AX_WRITE_DATA);
	sendData(AX_CW_COMPLIANCE_SLOPE);
    sendData(CWCSlope);
	sendData(AX_CCW_COMPLIANCE_SLOPE);
    sendData(CCWCSlope);
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int ShiftDynamixelClass::setPunch(unsigned char ID, int Punch)
{
    char Punch_H,Punch_L;
    Punch_H = Punch >> 8;           // 16 bits - 2 x 8 bits variables
    Punch_L = Punch;
	Checksum = (~(ID + AX_PUNCH_LENGTH + AX_WRITE_DATA + AX_PUNCH_L + Punch_L + Punch_H))&0xFF;
    
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                 // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_PUNCH_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_PUNCH_L);
    sendData(Punch_L);
    sendData(Punch_H);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    return (read_error());                 // Return the read error
}

int ShiftDynamixelClass::moving(unsigned char ID)
{	
    Checksum = (~(ID + AX_MOVING_LENGTH  + AX_READ_DATA + AX_MOVING + AX_BYTE_READ))&0xFF;
    
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_MOVING_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_MOVING);
    sendData(AX_BYTE_READ);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    Moving_Byte = -1;
    Time_Counter = 0;
    while((availableData() < 6) & (Time_Counter < TIME_OUT)){
		Time_Counter++;
		delayus(1000);
    }
	
    while (availableData() > 0){
		Incoming_Byte = readData();
		if ( (Incoming_Byte == 255) & (peekData() == 255) ){
			readData();                            // Start Bytes
			readData();                            // Ax-12 ID
			readData();                            // Length
			if( (Error_Byte = readData()) != 0 )   // Error
				return (Error_Byte*(-1));
			Moving_Byte = readData();         // Temperature
		}
    }
	return (Moving_Byte);               // Returns the read temperature
}

int ShiftDynamixelClass::lockRegister(unsigned char ID)
{    
	Checksum = (~(ID + AX_LR_LENGTH + AX_WRITE_DATA + AX_LOCK + LOCK))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_LR_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_LOCK);
    sendData(LOCK);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int ShiftDynamixelClass::RWStatus(unsigned char ID)
{	
    Checksum = (~(ID + AX_RWS_LENGTH  + AX_READ_DATA + AX_REGISTERED_INSTRUCTION + AX_BYTE_READ))&0xFF;
    
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_RWS_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_REGISTERED_INSTRUCTION);
    sendData(AX_BYTE_READ);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    RWS_Byte = -1;
    Time_Counter = 0;
    while((availableData() < 6) & (Time_Counter < TIME_OUT)){
		Time_Counter++;
		delayus(1000);
    }
	
    while (availableData() > 0){
		Incoming_Byte = readData();
		if ( (Incoming_Byte == 255) & (peekData() == 255) ){
			readData();                            // Start Bytes
			readData();                            // Ax-12 ID
			readData();                            // Length
			if( (Error_Byte = readData()) != 0 )   // Error
				return (Error_Byte*(-1));
			RWS_Byte = readData();         // Temperature
		}
    }
	return (RWS_Byte);               // Returns the read temperature
}

int ShiftDynamixelClass::readSpeed(unsigned char ID)
{	
    Checksum = (~(ID + AX_POS_LENGTH  + AX_READ_DATA + AX_PRESENT_SPEED_L + AX_BYTE_READ_POS))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_POS_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_SPEED_L);
    sendData(AX_BYTE_READ_POS);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    Speed_Long_Byte = -1;
	Time_Counter = 0;
    while((availableData() < 7) & (Time_Counter < TIME_OUT)){
		Time_Counter++;
		delayus(1000);
    }
	
    while (availableData() > 0){
		Incoming_Byte = readData();
		if ( (Incoming_Byte == 255) & (peekData() == 255) ){
			readData();                            // Start Bytes
			readData();                            // Ax-12 ID
			readData();                            // Length
			if( (Error_Byte = readData()) != 0 )   // Error
				return (Error_Byte*(-1));
			
			Speed_Low_Byte = readData();            // Position Bytes
			Speed_High_Byte = readData();
			Speed_Long_Byte = Speed_High_Byte << 8; 
			Speed_Long_Byte = Speed_Long_Byte + Speed_Low_Byte;
		}
    }
	return (Speed_Long_Byte);     // Returns the read position
}

int ShiftDynamixelClass::readLoad(unsigned char ID)
{	
    Checksum = (~(ID + AX_POS_LENGTH  + AX_READ_DATA + AX_PRESENT_LOAD_L + AX_BYTE_READ_POS))&0xFF;
	
	shift.write(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_POS_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_LOAD_L);
    sendData(AX_BYTE_READ_POS);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	shift.write(Direction_Pin,Rx_MODE);
	
    Load_Long_Byte = -1;
	Time_Counter = 0;
    while((availableData() < 7) & (Time_Counter < TIME_OUT)){
		Time_Counter++;
		delayus(1000);
    }
	
    while (availableData() > 0){
		Incoming_Byte = readData();
		if ( (Incoming_Byte == 255) & (peekData() == 255) ){
			readData();                            // Start Bytes
			readData();                            // Ax-12 ID
			readData();                            // Length
			if( (Error_Byte = readData()) != 0 )   // Error
				return (Error_Byte*(-1));
			
			Load_Low_Byte = readData();            // Position Bytes
			Load_High_Byte = readData();
			Load_Long_Byte = Load_High_Byte << 8; 
			Load_Long_Byte = Load_Long_Byte + Load_Low_Byte;
		}
    }
	return (Load_Long_Byte);     // Returns the read position
}

ShiftDynamixelClass ShiftDynamixel;



void ShiftRegAX12::SerialBegin(long baud, unsigned char rx, unsigned char tx, unsigned char control){
	ShiftDynamixel.begin(baud, rx, tx, control);
}

void ShiftRegAX12::end(){
	ShiftDynamixel.end();
}
	
void ShiftRegAX12::attach(unsigned char id){
	m_id = id;
}

void ShiftRegAX12::detach(){
	m_id = -1;
}

int ShiftRegAX12::ping(){
	return ShiftDynamixel.ping(m_id);
}
	
int ShiftRegAX12::setID(unsigned char newID){
	return ShiftDynamixel.setID(m_id, newID);
}
	
int ShiftRegAX12::setBD(long baud){
	return ShiftDynamixel.setBD(m_id, baud);
}

int ShiftRegAX12::move(float Position){
	if(m_endlessMode){
		setEndlessMode(OFF);
	}
	int pos = min(1023,Position/300*1023);
	return ShiftDynamixel.move(m_id, pos);
}

int ShiftRegAX12::moveSpeed(float Position, float speed){
	if(m_endlessMode){
		setEndlessMode(OFF);
	}
	int pos = min(1023,Position/300*1023);
	return ShiftDynamixel.moveSpeed(m_id, pos, speed);
}

int ShiftRegAX12::setEndlessMode(bool Status){
	m_endlessMode = Status;
	return ShiftDynamixel.setEndless(m_id, Status);
}
	
int ShiftRegAX12::turn(int Speed){
	if(Speed>0){
		int spd = min(1023, Speed);
		return ShiftDynamixel.turn(m_id, 0, spd);
	}
	else{
		int spd = max(-1023, Speed);
		return ShiftDynamixel.turn(m_id, 1, spd);
	}
}

int ShiftRegAX12::Nextmove(float Position){
	int pos = min(1023,Position/300*1023);
	return ShiftDynamixel.moveRW(m_id, pos);
}
	
int ShiftRegAX12::NextmoveSpeed(float Position, float Speed){
	int pos = min(1023, Position/300*1023);
	return ShiftDynamixel.moveSpeedRW(m_id, pos, Speed);
}
	
void ShiftRegAX12::action(){
	ShiftDynamixel.setEndless(254, OFF);
	ShiftDynamixel.action();
}

int ShiftRegAX12::setTempLimit(unsigned char Temperature){
	return ShiftDynamixel.setTempLimit(m_id, Temperature);
}
	
int ShiftRegAX12::setAngleLimit(float CW, float CCW){
	int CWLimit = min(1023, CW/300*1023);
	int CCWLimit = min(1023, CCW/300 *1023);
	return ShiftDynamixel.setAngleLimit(m_id, CWLimit, CCWLimit);
}

int ShiftRegAX12::setVoltageLimit(unsigned char DVoltage, unsigned char UVoltage){
	return ShiftDynamixel.setVoltageLimit(m_id, DVoltage*10, UVoltage*10);
}

int ShiftRegAX12::setMaxTorque(int MaxTorque){
	return ShiftDynamixel.setMaxTorque(m_id, MaxTorque);
}

int ShiftRegAX12::setMaxTorqueRAM(int MaxTorque){
	return ShiftDynamixel.setMaxTorqueRAM(m_id, MaxTorque);
}

int ShiftRegAX12::setSRL(unsigned char SRL){
	return ShiftDynamixel.setSRL(m_id, SRL);
}
	
int ShiftRegAX12::setRDT(unsigned char RDT){
	return ShiftDynamixel.setRDT(m_id, RDT);
}
	
int ShiftRegAX12::setLEDAlarm(unsigned char LEDAlarm){
	return ShiftDynamixel.setLEDAlarm(m_id, LEDAlarm);
}
	
int ShiftRegAX12::setShutdownAlarm(unsigned char SALARM){
	return ShiftDynamixel.setShutdownAlarm(m_id, SALARM);
}

int ShiftRegAX12::setCMargin(unsigned char CWCMargin, unsigned char CCWCMargin){
	return ShiftDynamixel.setCMargin(m_id, CWCMargin, CCWCMargin);
}
	
int ShiftRegAX12::setCSlope(unsigned char CWCSlope, unsigned char CCWCSlope){
	return ShiftDynamixel.setCSlope(m_id, CWCSlope, CCWCSlope);
}
	
int ShiftRegAX12::setPunch(int Punch){
	return ShiftDynamixel.setPunch(m_id, Punch);	
}

int ShiftRegAX12::moving(){
	return ShiftDynamixel.moving(m_id);
}
	
int ShiftRegAX12::lockRegister(){
	return ShiftDynamixel.lockRegister(m_id);
}
	
int ShiftRegAX12::savedMove(){
	return ShiftDynamixel.RWStatus(m_id);
}	

int ShiftRegAX12::readTemperature(){
	return ShiftDynamixel.readTemperature(m_id);
}
	
float ShiftRegAX12::readVoltage(){
	return ShiftDynamixel.readVoltage(m_id)/10;
}
	
float ShiftRegAX12::readPosition(){
	float pos = (float)(ShiftDynamixel.readPosition(m_id)) *300 /1023;
	return pos;
}

float ShiftRegAX12::readSpeed(){
	if(m_endlessMode){
		return ShiftDynamixel.readSpeed(m_id);
	}
	else{
		return ShiftDynamixel.readSpeed(m_id) /1023*114 ;
	}
}

int ShiftRegAX12::readTorque(){
	int torque = ShiftDynamixel.readLoad(m_id);
	if(torque <=1023){
		return map(torque, 0, 1023, 0, -1023);
	}
	else{
		return map(torque, 1024, 2047, 0, 1023);
	}
}
	
int ShiftRegAX12::hold(bool Status){
	m_holding = Status;
	return ShiftDynamixel.torqueStatus(m_id, Status);
}

bool ShiftRegAX12::isHolding(){
	return m_holding;
}

int ShiftRegAX12::led(bool Status){
	return ShiftDynamixel.ledStatus(m_id, Status);
}
