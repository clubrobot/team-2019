#include "Dynamixel.h"	
#include <Arduino.h>
#include "HardwareSerial.h"

HardwareSerial HardSerial(2);// Macro for the selection of the Serial Port

#define sendData(args)  (HardSerial.write(args))    // Write Over Serial
#define availableData() (HardSerial.available())    // Check Serial Data Available
#define readData()      (HardSerial.read())         // Read Serial Data
#define peekData()      ((unsigned byte) HardSerial.peek())         // Peek Serial Data
#define beginCom(args)  (HardSerial.begin(args))    // Begin Serial Comunication
#define endCom()        (HardSerial.end())          // End Serial Comunication

//#define setRXPin(args)  (SoftSerial.setRX(args))    // Set Rx Serial Pin
//#define setTXPin(args)  (SoftSerial.setTX(args))    // Set Tx Serial Pin

// Macro for Timing

#define delayus(args) (delayMicroseconds(args))  // Delay Microseconds

// Macro for Comunication Flow Control

#define setDPin(DirPin,Mode)   (pinMode(DirPin,Mode))       // Select the Switch to TX/RX Mode Pin

#define switchCom(DirPin,Mode) (digitalWrite(DirPin,Mode))  // Switch to TX/RX Mode

// Private Methods //////////////////////////////////////////////////////////////

int DynamixelClass::read_error(void)
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

void DynamixelClass::begin(long baud)
{	
	// DTx = Tx;
	// DRx = Rx;
	//setRXPin(DRx);
	//setTXPin(DTx);
	beginCom(baud);
	
}
void DynamixelClass::begin(long baud, unsigned char D_Pin)
{	
	// DTx = Tx;
	// DRx = Rx;
	//setRXPin(DRx);
	//setTXPin(DTx);
	beginCom(baud);
	pinMode(D_Pin,OUTPUT);
	Direction_Pin = D_Pin;	
}

void DynamixelClass::end()
{
	endCom();
}

int DynamixelClass::reset(unsigned char ID)
{
	Checksum = (~(ID + AX_RESET_LENGTH + AX_RESET))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_RESET_LENGTH);
	sendData(AX_RESET);    
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);

    return (read_error());  
}

int DynamixelClass::ping(unsigned char ID)
{
	Checksum = (~(ID + AX_READ_DATA + AX_PING))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_READ_DATA);
	sendData(AX_PING);    
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());              
}

int DynamixelClass::setID(unsigned char ID, unsigned char newID)
{    
	Checksum = (~(ID + AX_ID_LENGTH + AX_WRITE_DATA + AX_ID + newID))&0xFF;

	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_ID_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_ID);
    sendData(newID);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int DynamixelClass::setBD(unsigned char ID, long baud)
{    
	unsigned char Baud_Rate = (2000000/baud) - 1;
    Checksum = (~(ID + AX_BD_LENGTH + AX_WRITE_DATA + AX_BAUD_RATE + Baud_Rate))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);                 // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_BD_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_BAUD_RATE);
    sendData(Baud_Rate);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int DynamixelClass::move(unsigned char ID, int Position)
{
    char Position_H,Position_L;
    Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
    Position_L = Position;
	Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;
    
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);

    return (read_error());                 // Return the read error
}

int DynamixelClass::moveSpeed(unsigned char ID, int Position, int Speed)
{
    char Position_H,Position_L,Speed_H,Speed_L;
    Position_H = Position >> 8;    
    Position_L = Position;                // 16 bits - 2 x 8 bits variables
    Speed_H = Speed >> 8;
    Speed_L = Speed;                      // 16 bits - 2 x 8 bits variables
	Checksum = (~(ID + AX_GOAL_SP_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H + Speed_L + Speed_H))&0xFF;
 
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());               // Return the read error
}

int DynamixelClass::setEndless(unsigned char ID, bool Status)
{
 if ( Status ) {	
	  char AX_CCW_AL_LT = 0;     // Changing the CCW Angle Limits for Full Rotation.
	  Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L))&0xFF;
	
	  switchCom(Direction_Pin,Tx_MODE);
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
	  switchCom(Direction_Pin,Rx_MODE);

	  return(read_error());
 }
 else
 {
	 turn(ID,0,0);
	 Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L + AX_CCW_AL_L + AX_CCW_AL_H))&0xFF;
	
	 switchCom(Direction_Pin,Tx_MODE);
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
	 switchCom(Direction_Pin,Rx_MODE);
	 
	 return (read_error());                 // Return the read error
  }
 } 

int DynamixelClass::turn(unsigned char ID, bool SIDE, int Speed)
{		
		if (SIDE == 0){                          // Move Left///////////////////////////
			
			char Speed_H,Speed_L;
			Speed_H = Speed >> 8;
			Speed_L = Speed;                     // 16 bits - 2 x 8 bits variables
			Checksum = (~(ID + AX_SPEED_LENGTH + AX_WRITE_DATA + AX_GOAL_SPEED_L + Speed_L + Speed_H))&0xFF;
			
			switchCom(Direction_Pin,Tx_MODE);
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
			switchCom(Direction_Pin,Rx_MODE);
			
			return(read_error());               // Return the read error		
		}
		else
		{                                            // Move Rigth////////////////////
			char Speed_H,Speed_L;
			Speed_H = (Speed >> 8) + 4;
			Speed_L = Speed;                     // 16 bits - 2 x 8 bits variables
			Checksum = (~(ID + AX_SPEED_LENGTH + AX_WRITE_DATA + AX_GOAL_SPEED_L + Speed_L + Speed_H))&0xFF;
			
			switchCom(Direction_Pin,Tx_MODE);
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
			switchCom(Direction_Pin,Rx_MODE);
			
			return(read_error());                // Return the read error	
		}
}

int DynamixelClass::moveRW(unsigned char ID, int Position)
{
    char Position_H,Position_L;
    Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
    Position_L = Position;
    Checksum = (~(ID + AX_GOAL_LENGTH + AX_REG_WRITE + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;

	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
	
    return (read_error());                 // Return the read error
}

int DynamixelClass::moveSpeedRW(unsigned char ID, int Position, int Speed)
{
    char Position_H,Position_L,Speed_H,Speed_L;
    Position_H = Position >> 8;    
    Position_L = Position;                // 16 bits - 2 x 8 bits variables
    Speed_H = Speed >> 8;
    Speed_L = Speed;                      // 16 bits - 2 x 8 bits variables
    Checksum = (~(ID + AX_GOAL_SP_LENGTH + AX_REG_WRITE + AX_GOAL_POSITION_L + Position_L + Position_H + Speed_L + Speed_H))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());               // Return the read error
}

void DynamixelClass::action()
{	
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(BROADCAST_ID);
    sendData(AX_ACTION_LENGTH);
    sendData(AX_ACTION);
    sendData(AX_ACTION_CHECKSUM);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
}

int DynamixelClass::torqueStatus( unsigned char ID, bool Status)
{
    Checksum = (~(ID + AX_TORQUE_LENGTH + AX_WRITE_DATA + AX_TORQUE_ENABLE + Status))&0xFF;

	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);              // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_TORQUE_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_TORQUE_ENABLE);
    sendData(Status);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());              // Return the read error
}

int DynamixelClass::ledStatus(unsigned char ID, bool Status)
{    
    Checksum = (~(ID + AX_LED_LENGTH + AX_WRITE_DATA + AX_LED + Status))&0xFF;

	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);              // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
    sendData(AX_LED_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_LED);
    sendData(Status);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());              // Return the read error
}

int DynamixelClass::readTemperature(unsigned char ID)
{	
    Checksum = (~(ID + AX_TEM_LENGTH  + AX_READ_DATA + AX_PRESENT_TEMPERATURE + AX_BYTE_READ))&0xFF;
    
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_TEM_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_TEMPERATURE);
    sendData(AX_BYTE_READ);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
	
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

int DynamixelClass::readPosition(unsigned char ID)
{	
    Checksum = (~(ID + AX_POS_LENGTH  + AX_READ_DATA + AX_PRESENT_POSITION_L + AX_BYTE_READ_POS))&0xFF;
  
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_POS_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_POSITION_L);
    sendData(AX_BYTE_READ_POS);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
	
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

int DynamixelClass::readVoltage(unsigned char ID)
{    
    Checksum = (~(ID + AX_VOLT_LENGTH  + AX_READ_DATA + AX_PRESENT_VOLTAGE + AX_BYTE_READ))&0xFF;
    
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_VOLT_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_VOLTAGE);
    sendData(AX_BYTE_READ);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
	
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

int DynamixelClass::setTempLimit(unsigned char ID, unsigned char Temperature)
{
	Checksum = (~(ID + AX_TL_LENGTH +AX_WRITE_DATA+ AX_LIMIT_TEMPERATURE + Temperature))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
	sendData(AX_START);                     
	sendData(AX_START);
	sendData(ID);
	sendData(AX_TL_LENGTH);
	sendData(AX_WRITE_DATA);
	sendData(AX_LIMIT_TEMPERATURE);
    sendData(Temperature);
	sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int DynamixelClass::setVoltageLimit(unsigned char ID, unsigned char DVoltage, unsigned char UVoltage)
{
	Checksum = (~(ID + AX_VL_LENGTH +AX_WRITE_DATA+ AX_DOWN_LIMIT_VOLTAGE + DVoltage + UVoltage))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int DynamixelClass::setAngleLimit(unsigned char ID, int CWLimit, int CCWLimit)
{
	char CW_H,CW_L,CCW_H,CCW_L;
    CW_H = CWLimit >> 8;    
    CW_L = CWLimit;                // 16 bits - 2 x 8 bits variables
    CCW_H = CCWLimit >> 8;
    CCW_L = CCWLimit;  
	Checksum = (~(ID + AX_VL_LENGTH +AX_WRITE_DATA+ AX_CW_ANGLE_LIMIT_L + CW_H + CW_L + AX_CCW_ANGLE_LIMIT_L + CCW_H + CCW_L))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int DynamixelClass::setMaxTorque(unsigned char ID, int MaxTorque)
{
    char MaxTorque_H,MaxTorque_L;
    MaxTorque_H = MaxTorque >> 8;           // 16 bits - 2 x 8 bits variables
    MaxTorque_L = MaxTorque;
	Checksum = (~(ID + AX_MT_LENGTH + AX_WRITE_DATA + AX_MAX_TORQUE_L + MaxTorque_L + MaxTorque_H))&0xFF;
    
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
	
    return (read_error());                 // Return the read error
}

int DynamixelClass::setMaxTorqueRAM(unsigned char ID, int MaxTorque)
{
    char MaxTorque_H,MaxTorque_L;
    MaxTorque_H = MaxTorque >> 8;           // 16 bits - 2 x 8 bits variables
    MaxTorque_L = MaxTorque;
	Checksum = (~(ID + AX_MT_LENGTH + AX_WRITE_DATA + 0x22 + MaxTorque_L + MaxTorque_H))&0xFF;
    
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
	
    return (read_error());                 // Return the read error
}

int DynamixelClass::setSRL(unsigned char ID, unsigned char SRL)
{    
	Checksum = (~(ID + AX_SRL_LENGTH + AX_WRITE_DATA + AX_RETURN_LEVEL + SRL))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_SRL_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_RETURN_LEVEL);
    sendData(SRL);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int DynamixelClass::setRDT(unsigned char ID, unsigned char RDT)
{    
	Checksum = (~(ID + AX_RDT_LENGTH + AX_WRITE_DATA + AX_RETURN_DELAY_TIME + (RDT/2)))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_RDT_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_RETURN_DELAY_TIME);
    sendData((RDT/2));
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int DynamixelClass::setLEDAlarm(unsigned char ID, unsigned char LEDAlarm)
{    
	Checksum = (~(ID + AX_LEDALARM_LENGTH + AX_WRITE_DATA + AX_ALARM_LED + LEDAlarm))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_LEDALARM_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_ALARM_LED);
    sendData(LEDAlarm);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int DynamixelClass::setShutdownAlarm(unsigned char ID, unsigned char SALARM)
{    
	Checksum = (~(ID + AX_SALARM_LENGTH + AX_WRITE_DATA + AX_ALARM_SHUTDOWN + SALARM))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_SALARM_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_ALARM_SHUTDOWN);
    sendData(SALARM);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int DynamixelClass::setCMargin(unsigned char ID, unsigned char CWCMargin, unsigned char CCWCMargin)
{
	Checksum = (~(ID + AX_CM_LENGTH +AX_WRITE_DATA+ AX_CW_COMPLIANCE_MARGIN + CWCMargin + AX_CCW_COMPLIANCE_MARGIN + CCWCMargin))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int DynamixelClass::setCSlope(unsigned char ID, unsigned char CWCSlope, unsigned char CCWCSlope)
{
	Checksum = (~(ID + AX_CS_LENGTH +AX_WRITE_DATA+ AX_CW_COMPLIANCE_SLOPE + CWCSlope + AX_CCW_COMPLIANCE_SLOPE + CCWCSlope))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
	
    return (read_error()); 
}

int DynamixelClass::setPunch(unsigned char ID, int Punch)
{
    char Punch_H,Punch_L;
    Punch_H = Punch >> 8;           // 16 bits - 2 x 8 bits variables
    Punch_L = Punch;
	Checksum = (~(ID + AX_PUNCH_LENGTH + AX_WRITE_DATA + AX_PUNCH_L + Punch_L + Punch_H))&0xFF;
    
	switchCom(Direction_Pin,Tx_MODE);
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
	switchCom(Direction_Pin,Rx_MODE);
	
    return (read_error());                 // Return the read error
}

int DynamixelClass::moving(unsigned char ID)
{	
    Checksum = (~(ID + AX_MOVING_LENGTH  + AX_READ_DATA + AX_MOVING + AX_BYTE_READ))&0xFF;
    
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_MOVING_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_MOVING);
    sendData(AX_BYTE_READ);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
	
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

int DynamixelClass::lockRegister(unsigned char ID)
{    
	Checksum = (~(ID + AX_LR_LENGTH + AX_WRITE_DATA + AX_LOCK + LOCK))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);                // Send Instructions over Serial
    sendData(AX_START);
    sendData(ID);
	sendData(AX_LR_LENGTH);
    sendData(AX_WRITE_DATA);
    sendData(AX_LOCK);
    sendData(LOCK);
    sendData(Checksum);
	delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
    
    return (read_error());                // Return the read error
}

int DynamixelClass::RWStatus(unsigned char ID)
{	
    Checksum = (~(ID + AX_RWS_LENGTH  + AX_READ_DATA + AX_REGISTERED_INSTRUCTION + AX_BYTE_READ))&0xFF;
    
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_RWS_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_REGISTERED_INSTRUCTION);
    sendData(AX_BYTE_READ);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
	
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

int DynamixelClass::readSpeed(unsigned char ID)
{	
    Checksum = (~(ID + AX_POS_LENGTH  + AX_READ_DATA + AX_PRESENT_SPEED_L + AX_BYTE_READ_POS))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_POS_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_SPEED_L);
    sendData(AX_BYTE_READ_POS);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
	
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

int DynamixelClass::readLoad(unsigned char ID)
{	
    Checksum = (~(ID + AX_POS_LENGTH  + AX_READ_DATA + AX_PRESENT_LOAD_L + AX_BYTE_READ_POS))&0xFF;
	
	switchCom(Direction_Pin,Tx_MODE);
    sendData(AX_START);
    sendData(AX_START);
    sendData(ID);
    sendData(AX_POS_LENGTH);
    sendData(AX_READ_DATA);
    sendData(AX_PRESENT_LOAD_L);
    sendData(AX_BYTE_READ_POS);
    sendData(Checksum);
    delayus(TX_DELAY_TIME);
	switchCom(Direction_Pin,Rx_MODE);
	
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

DynamixelClass Dynamixel; // Global Instance 