#include "ExperienceEffects.h"

/* Constuctor */
ExperienceEffects::ExperienceEffects()
{
	hasStarted = 0;
	//FastLED.addLeds<CHIPSET, LOGO_PIN, COLOR_ORDER>(leds,34).setCorrection(TypicalSMD5050);

	FastLED.addLeds<NEOPIXEL, 6>(leds, 60);
}


/* public methodes */

void ExperienceEffects::start()
{
  for (int i = 0; i < 60; i++){
		leds[i] = CRGB::Red;
	}
	FastLED.show();
	hasStarted = 1;
}

long ExperienceEffects::getStart(){
	return hasStarted;
}



void ExperienceEffects::execute()
{

}
