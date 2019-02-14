#include <Arduino.h>
#include "NeoPixel/Adafruit_NeoPixel.h"

#define NUMPIXEL 60
#define PINPIXEL 34

class ExperienceEffects
{
	public:
		ExperienceEffects();

		void start();
		long getStart();
		void execute();

	private:

		Adafruit_NeoPixel logo;
		long hasStarted;

};
