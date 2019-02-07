#include "ExperienceEffects.h"

/* Constuctor */
ExperienceEffects::ExperienceEffects()
{
	logo = Adafruit_NeoPixel(NUMPIXEL, PINPIXEL, NEO_GRB+NEO_KHZ800);
	hasStarted = 0;
}


/* public methodes */

void ExperienceEffects::start()
{
    for (int i = 0; i < logo.numPixels(); i++){
		logo.setPixelColor(i, logo.Color(0, 250, 0));
	}
	hasStarted = 1;
}

bool ExperienceEffects::getStart(){
	return hasStarted;
}



void ExperienceEffects::execute()
{

}
