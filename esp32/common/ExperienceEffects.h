#include <Arduino.h>
#include "NeoPixel/Adafruit_NeoPixel.h"
#include "/Users/apegne/Documents/Arduino/libraries/FastLED/FastLED.h"

#define COLOR_ORDER BRG
#define CHIPSET     WS2811
#define NUMPIXEL 60
#define PINPIXEL 34
#define PWM_MOTOR 21
#define EN_MOTOR 27
#define SELECT_MOTOR 12
#define SERVO1 32
#define SERVO2 25
#define ROTARY 4
#define RST_DC_DRV 23

class ExperienceEffects
{
	public:
		ExperienceEffects();

		void start();
		long getStart();
		void execute();
		void setup();

	private:


		CRGB leds[NUMPIXEL];
		long hasStarted;

};
