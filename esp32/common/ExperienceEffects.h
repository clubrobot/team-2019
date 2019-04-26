#include <Arduino.h>
#include "../common/fastLED/FastLED.h"
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

#define BUILTIN_LED		2

#define SERVO1 				32
#define SERVO2 				25

#define ROTARY 				4

#define INTERRUPT			4

<<<<<<< HEAD
=======
#define TEMPS_MIN			80

>>>>>>> 18901e37063db032ed2bf8874ff69ea1cba8e4f7
#define GO_BACK				32
#define GO_FORWARD		25


class ExperienceEffects
{
	public:
		ExperienceEffects(boolean);

		void start();
		void connected();
		void execute();
		void setup();
		void launch();
		void stayOnTop();
		long getTimer();
		boolean getOnTop();
		long getStart();
		bool isElectron;
		void goBack();
		void goForward();
		void motorStop();

	private:
		bool isOnTop;
		CRGB leds[NUMPIXEL];
		long hasStarted;
		DCMotor motor;
		DCMotorsDriver driver;
		long timer;

};
