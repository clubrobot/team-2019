#ifndef __NEW_AX12
#define __NEW_AX12

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
#define TIME_OUT                    10         // Este parametro depende de la velocidad de transmision
#define TX_DELAY_TIME				400        // Este parametro depende de la velocidad de transmision - pero pueden ser cambiados para mayor velocidad.
#define Tx_MODE                     1
#define Rx_MODE                     0
#define LOCK                        1

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

class DynamixelClass 
{
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
        
        int Moving_Byte;
        int RWS_Byte;
        int Speed_Long_Byte;
        int Load_Long_Byte;
        int Position_Long_Byte;
        int Temperature_Byte;
        int Voltage_Byte;
        int Error_Byte; 
        
        void save_error(void);
        
    public:
        
        void begin(long baud);
        void begin(long baud, unsigned char D_Pin);
        void end(void);

        int get_error(void);
        
        void reset(unsigned char ID);
        
        void setID(unsigned char ID, unsigned char newID);
        void setBD(unsigned char ID, long baud);
        
        void move(unsigned char ID, int Position);
        void moveSpeed(unsigned char ID, int Position, int Speed);
        void setEndless(unsigned char ID,bool Status);
        void turn(unsigned char ID, bool SIDE, int Speed);
        void moveRW(unsigned char ID, int Position);
        void moveSpeedRW(unsigned char ID, int Position, int Speed);
        
        void action(void);
        
        void setTempLimit(unsigned char ID, unsigned char Temperature);
        void setAngleLimit(unsigned char ID, int CWLimit, int CCWLimit);
        void setVoltageLimit(unsigned char ID, unsigned char DVoltage, unsigned char UVoltage);
        void setMaxTorque(unsigned char ID, int MaxTorque);
        void setMaxTorqueRAM(unsigned char ID, int MaxTorque);
        void setSRL(unsigned char ID, unsigned char SRL);
        void setRDT(unsigned char ID, unsigned char RDT);
        void setLEDAlarm(unsigned char ID, unsigned char LEDAlarm);
        void setShutdownAlarm(unsigned char ID, unsigned char SALARM);
        void setCMargin(unsigned char ID, unsigned char CWCMargin, unsigned char CCWCMargin);
        void setCSlope(unsigned char ID, unsigned char CWCSlope, unsigned char CCWCSlope);
        void setPunch(unsigned char ID, int Punch);
        
        int moving(unsigned char ID);
        void lockRegister(unsigned char ID);
        int RWStatus(unsigned char ID);
        
        int readTemperature(unsigned char ID);
        int readVoltage(unsigned char ID);
        int readPosition(unsigned char ID);
        int readSpeed(unsigned char ID);
        int readLoad(unsigned char ID);
        
        void torqueStatus(unsigned char ID, bool Status);
        void ledStatus(unsigned char ID, bool Status);
};

extern DynamixelClass Dynamixel;

class AX12 {

	private: 

		unsigned char m_id;
		bool m_endlessMode;
		bool m_holding;

	public:

		static void SerialBegin(long baud, unsigned char control);
		static void end();

        int get_error();

		void attach(unsigned char id);
		void detach();
		
		void setID(unsigned char newID);
		void setBD(long baud);

		void move(float Position);
		void moveSpeed(float Position, float Speed);

		void setEndlessMode(bool Status);
		void turn(int Speed);
		
		void Nextmove(float Position);
		void NextmoveSpeed(float Position, float Speed);
		
		void static action();

		void setTempLimit(unsigned char Temperature);
		void setAngleLimit(float CWLimit, float CCWLimit);
		void setVoltageLimit(unsigned char DVoltage, unsigned char UVoltage);
		void setMaxTorque(int MaxTorque);
		void setMaxTorqueRAM(int MaxTorque);
		void setSRL(unsigned char SRL);
		void setRDT(unsigned char RDT);
		void setLEDAlarm(unsigned char LEDAlarm);
		void setShutdownAlarm(unsigned char SALARM);
		void setCMargin(unsigned char CWCMargin, unsigned char CCWCMargin);
		void setCSlope( unsigned char CWCSlope, unsigned char CCWCSlope);
		void setPunch(int Punch);

		int moving();
		void lockRegister();
		int savedMove();
		
		int readTemperature();
		double readVoltage();
		double readPosition();
		double readSpeed();
		double readTorque();

		bool isHolding();
		
		int hold(bool Status);
		int led(bool Status);
	
};



#endif /* __NEW_AX12 */