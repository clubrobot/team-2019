#include <Arduino.h>
#include "/Users/apegne/Documents/Arduino/libraries/FastLED/FastLED.h"
#include "../common/ESP32-Arduino-Servo-Library/src/Servo.h"
#include "../common/DCMotor.h"

#define NUMPIXEL 			60
#define PINPIXEL 			33

#define PWM_MOTOR 		21
#define PWMCHANNEL 		0
#define EN_MOTOR 			27
#define SELECT_MOTOR 	12
#define RST_DC_DRV 		23
#define FLT_DC_DRV		0

#define SERVO1 				32
#define SERVO2 				25

#define ROTARY 				4

#define INTERRUPT			5

#define TEMPS_MIN			30


class ExperienceEffects
{
	public:
		ExperienceEffects(boolean);

		void start();
		void execute();
		void setup();
		void launch();
		void stayOnTop();
		long getTimer();
		boolean getOnTop();
		long getStart();
		bool isElectron;

	private:
		bool isOnTop;
		CRGB leds[NUMPIXEL];
		long hasStarted;
		DCMotor motor;
		DCMotorsDriver driver;
		long timer;

};
