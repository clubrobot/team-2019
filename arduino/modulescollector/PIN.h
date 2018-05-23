#ifndef __PIN_H__
#define __PIN_H__

#include <Arduino.h>

/*
Pin arduino carte actionneurs PROTO

-----------------------------------------
INTER	1	2	3	4	5
pin	2	4	7	8	13
-----------------------------------------
Driver moteur		Pin
PWM_moteur_1		3
Select_moteur_1		5
EN_moteur_1		6
PWM_moteur_2		11
Select_moteur_2		10
EN_moteur_2		9
Reset_driver		1
-----------------------------------------
SERVO	1	2	3	4
pin	A5	A4	A3	A2
*/

/* 
Carte V1.0

Servo : 
SERVO_1 A5
SERVO_2 A4
SERVO_3 A3
SERVO_4 A2

Serial : 
RX 		A0
TX 		A1
DATA_CONTROL	13

EndStop : 
INTER_1		2
INTER_2		4

Driver : 
RESET_DRIVER	12


Moteur: 
PWN_MOTEUR_1 	3
SELECT_MOTEUR_1 5
EN_MOTEUR_1 	6

PWN_MOTEUR_2	9
SELECT_MOTEUR_2 10
PWM_MOTEUR_2	11

TOR: 
TOR_1 		7
TOR_2 		8

*/

//Proto
// DC motors driver
/*
#define MOTOR2_EN       6
#define MOTOR2_PWM      3
#define MOTOR2_DIR      5
#define MOTOR1_EN       9
#define MOTOR1_PWM      11
#define MOTOR1_DIR      10
#define DRIVER_RESET    12

// Switchs

#define SWITCH1 2
#define SWITCH2 4
#define SWITCH3 7
#define SWITCH4 8
#define SWITCH5 13

*/
// Servos

#define SERVO1 A4
#define SERVO2 A5
#define SERVO3 A3
#define SERVO4 A2




//V1.0
//Serial : 
#define RX 		A0
#define TX 		A1
#define DATA_CONTROL	13

//EndStop : 
#define SWITCH1		2
#define SWITCH2		4

//Driver : 
#define DRIVER_RESET	12


//Moteur: 
#define MOTOR1_EN       6
#define MOTOR1_PWM      3
#define MOTOR1_DIR      5
#define MOTOR2_EN       9
#define MOTOR2_PWM      11
#define MOTOR2_DIR      10

//TOR: 
#define TOR_1 		7
#define TOR_2 		8



#endif // __PIN_H__
