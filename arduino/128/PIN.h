#ifndef __PIN_H__
#define __PIN_H__

#include <Arduino.h>
/*|**************************|***************************|***************************|
  |*** SHIFT REGISTER PINS **|******** SERVO PINS *******|****** DC MOTOR PINS ******|
  |**************************|***************************|***************************|
  | LATCHPIN 10 - OUT		 | SERVO1 A2 - OUT			 | PWM_MOT_1 3  - OUT		 |
  | CLOCKPIN 12 - OUT		 | SERVO2 A3 - OUT			 | PWM_MOT_2 11 - OUT		 |
  | DATAPIN  5  - OUT		 | SERVO3 A4 - OUT			 |							 |
  |	*	*	*	*	*	*	 | SERVO4 A5 - OUT			 | EN_MOT_1 6   - OUT		 |
  | QA(0): AX12_DATA_CONTROL | SERVO5 7	 - OUT			 | EN_MOT_2 9   - OUT		 |
  | QB(1): RST_DRV		   	 | SERVO6 8	 - OUT			 |							 |
  | QC(2): SELECT_MOTOR1  	 |***************************|****************************
  | QD(3): SELECT_MOTOR2  	 |******** PAP PINS *********|	
  | QE(4): DIR_PAP			 |***************************|
  | QF(5): SLEEP_PAP 		 | 							 |							
  | QG(6): RST_PAP			 |STEP_PAP 13 - OUT			 |		
  | QH(7): ENABLE_PAP  		 |							 |
  |**************************|***************************|
  |******* INTER PINS *******|******** AX12 PINS ********|
  |**************************|***************************|  			
  | INTER1 4	- IN		 | RX_AX12 A0 - IN 			 |
  | INTER2 2	- IN		 | TX_AX12 A1 - OUT 		 |
  ********************************************************/

#define LATCHPIN	10
#define CLOCKPIN	12
#define DATAPIN		5 
 
#define AX12_DATA_CONTROL	0
#define RST_DRV				1
#define SELECT_MOTOR1		2
#define SELECT_MOTOR2		3
#define DIR_PAP				4
#define SLEEP_PAP			5
#define RST_PAP				6
#define ENABLE_PAP			7

#define STEP_PAP 13

#define RX_AX12 A0
#define TX_AX12 A1

#define SERVO_BEE	A2
#define SERVO2  A3
#define SERVO3  A4
#define SERVO4  A5
#define SERVO_GRIPPER 7
#define SERVO6  8

#define INTER1 4
#define INTER2 2

#define EN_MOTOR_1  6
#define EN_MOTOR_2	9

#define PWM_MOTOR_1	3
#define PWM_MOTOR_2	11

#endif // __PIN_H__