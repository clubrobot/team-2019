#include "ExperienceEffects.h"

/* Constuctor */
ExperienceEffects::ExperienceEffects()
{
	hasStarted = 0;
	//FastLED.addLeds<NEOPIXEL, 10, COLOR_ORDER>(leds,10).setCorrection(TypicalSMD5050);

	FastLED.addLeds<NEOPIXEL, 33>(leds, 60);
}


/* public methodes */
void ExperienceEffects::setup()
{
  for (int i = 0; i < 60; i++){
		leds[i] = CRGB::Black;
	}
	FastLED.show();
}

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
