#ifndef __DYNAMIXEL_H
#define __DYNAMIXEL_H

#include <inttypes.h>
#include <Arduino.h>

	/// EEPROM AREA  ///
#define AX_MODEL_NUMBER_L           0
#define AX_MODEL_NUMBER_H           1
#define AX_VERSION                  2
#define AX_ID                       3
#define AX_BAUD_RATE                4
#define AX_RETURN_DELAY_TIME        5
#define AX_CW_ANGLE_LIMIT_L         6
#define AX_CW_ANGLE_LIMIT_H         7
#define AX_CCW_ANGLE_LIMIT_L        8
#define AX_CCW_ANGLE_LIMIT_H        9
#define AX_SYSTEM_DATA2             10
#define AX_LIMIT_TEMPERATURE        11
#define AX_DOWN_LIMIT_VOLTAGE       12
#define AX_UP_LIMIT_VOLTAGE         13
#define AX_MAX_TORQUE_L             14
#define AX_MAX_TORQUE_H             15
#define AX_RETURN_LEVEL             16
#define AX_ALARM_LED                17
#define AX_ALARM_SHUTDOWN           18
#define AX_OPERATING_MODE           19
#define AX_DOWN_CALIBRATION_L       20
#define AX_DOWN_CALIBRATION_H       21
#define AX_UP_CALIBRATION_L         22
#define AX_UP_CALIBRATION_H         23

	/// RAM AREA  ////
#define AX_TORQUE_ENABLE            24
#define AX_LED                      25
#define AX_CW_COMPLIANCE_MARGIN     26
#define AX_CCW_COMPLIANCE_MARGIN    27
#define AX_CW_COMPLIANCE_SLOPE      28
#define AX_CCW_COMPLIANCE_SLOPE     29
#define AX_GOAL_POSITION_L          30
#define AX_GOAL_POSITION_H          31
#define AX_GOAL_SPEED_L             32
#define AX_GOAL_SPEED_H             33
#define AX_TORQUE_LIMIT_L           34
#define AX_TORQUE_LIMIT_H           35
#define AX_PRESENT_POSITION_L       36
#define AX_PRESENT_POSITION_H       37
#define AX_PRESENT_SPEED_L          38
#define AX_PRESENT_SPEED_H          39
#define AX_PRESENT_LOAD_L           40
#define AX_PRESENT_LOAD_H           41
#define AX_PRESENT_VOLTAGE          42
#define AX_PRESENT_TEMPERATURE      43
#define AX_REGISTERED_INSTRUCTION   44
#define AX_PAUSE_TIME               45
#define AX_MOVING                   46
#define AX_LOCK                     47
#define AX_PUNCH_L                  48
#define AX_PUNCH_H                  49

   /// Status Return Levels ///
#define AX_RETURN_NONE              0
#define AX_RETURN_READ              1
#define AX_RETURN_ALL               2

	/// Instruction Set ///
#define AX_PING                     1
#define AX_READ_DATA                2
#define AX_WRITE_DATA               3
#define AX_REG_WRITE                4
#define AX_ACTION                   5
#define AX_RESET                    6
#define AX_SYNC_WRITE               131

	/// Specials ///
#define OFF                         0
#define ON                          1
#define LEFT						0
#define RIGTH                       1
#define AX_BYTE_READ                1
#define AX_BYTE_READ_POS            2
#define AX_RESET_LENGTH				2
#define AX_ACTION_LENGTH			2
#define AX_ID_LENGTH                4
#define AX_LR_LENGTH                4
#define AX_SRL_LENGTH               4
#define AX_RDT_LENGTH               4
#define AX_LEDALARM_LENGTH          4
#define AX_SALARM_LENGTH            4
#define AX_TL_LENGTH                4
#define AX_VL_LENGTH                6
#define AX_CM_LENGTH                6
#define AX_CS_LENGTH                6
#define AX_CCW_CW_LENGTH            8
#define AX_BD_LENGTH                4
#define AX_TEM_LENGTH               4
#define AX_MOVING_LENGTH            4
#define AX_RWS_LENGTH               4
#define AX_VOLT_LENGTH              4
#define AX_LED_LENGTH               4
#define AX_TORQUE_LENGTH            4
#define AX_POS_LENGTH               4
#define AX_GOAL_LENGTH              5
#define AX_MT_LENGTH                5
#define AX_PUNCH_LENGTH             5
#define AX_SPEED_LENGTH             5
#define AX_GOAL_SP_LENGTH           7
#define AX_ACTION_CHECKSUM			250
#define BROADCAST_ID                254
#define AX_START                    255
#define AX_CCW_AL_L                 255 
#define AX_CCW_AL_H                 3
#define TIME_OUT                    1000        // Este parametro depende de la velocidad de transmision
#define TX_DELAY_TIME				400        // Este parametro depende de la velocidad de transmision - pero pueden ser cambiados para mayor velocidad.
#define Tx_MODE                     1
#define Rx_MODE                     0
#define LOCK                        1

class DynamixelClass {
private:

	unsigned char Checksum; 
	unsigned char Direction_Pin;
	unsigned char Time_Counter;
	unsigned char Incoming_Byte;               
	unsigned char Position_High_Byte;
	unsigned char Position_Low_Byte;
	unsigned char Speed_High_Byte;
	unsigned char Speed_Low_Byte;
	unsigned char Load_High_Byte;
	unsigned char Load_Low_Byte;
	  
	int readDatafromAX(unsigned char id, int offset);
	
public:

	void begin(long baud);
	void begin(long baud, unsigned char D_Pin);
	void end(void);
	
	int reset(unsigned char ID);
	int ping(unsigned char ID); 
	
	int setID(unsigned char ID, unsigned char newID);
	int setBD(unsigned char ID, long baud);
	
	int move(unsigned char ID, int Position);
	int moveSpeed(unsigned char ID, int Position, int Speed);
	int setEndless(unsigned char ID,bool Status);
	int turn(unsigned char ID, bool SIDE, int Speed);
	int moveRW(unsigned char ID, int Position);
	int moveSpeedRW(unsigned char ID, int Position, int Speed);
	
	void action(void);
	
	int setTempLimit(unsigned char ID, unsigned char Temperature);
	int setAngleLimit(unsigned char ID, int CWLimit, int CCWLimit);
	int setVoltageLimit(unsigned char ID, unsigned char DVoltage, unsigned char UVoltage);
	int setMaxTorque(unsigned char ID, int MaxTorque);
	int setMaxTorqueRAM(unsigned char ID, int MaxTorque);
	int setSRL(unsigned char ID, unsigned char SRL);
	int setRDT(unsigned char ID, unsigned char RDT);
	int setLEDAlarm(unsigned char ID, unsigned char LEDAlarm);
	int setShutdownAlarm(unsigned char ID, unsigned char SALARM);
	int setCMargin(unsigned char ID, unsigned char CWCMargin, unsigned char CCWCMargin);
	int setCSlope(unsigned char ID, unsigned char CWCSlope, unsigned char CCWCSlope);
	int setPunch(unsigned char ID, int Punch);
	
	int moving(unsigned char ID);
	int lockRegister(unsigned char ID);
	int RWStatus(unsigned char ID);
	
	int readTemperature(unsigned char ID);
	int readVoltage(unsigned char ID);
	int readPosition(unsigned char ID);
	int readSpeed(unsigned char ID);
	int readLoad(unsigned char ID);
	
	int torqueStatus(unsigned char ID, bool Status);
	int ledStatus(unsigned char ID, bool Status);
};

extern DynamixelClass Dynamixel;

typedef enum
{
	INPUT_VOLTAGE 	= 1,
	ANGLE_LIMIT		= 2,
	OVERHEATING		= 4,
	RANGE			= 8,
	CHECKSUM		= 16,
	OVERLOAD		= 32,
	INSTRUCTION		= 64,
}ax_error_t;

class AX12Timeout
{
	public:
    	AX12Timeout(int id):m_id(id){}
		int get_id() const {return m_id;}
	private:
		int m_id;
};

class AX12error
{
    public :
        AX12error(int ID, int error_code): m_id(ID), m_error_code(error_code){}

        bool resolve_AX_error();
		int get_id() const {return m_id;}
    
    private :
        
        int m_id;
        int m_error_code;
};
#endif