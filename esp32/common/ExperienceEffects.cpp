#include "ExperienceEffects.h"

/* Constuctor */
ExperienceEffects::ExperienceEffects(boolean electron){
	hasStarted = 0;
	timer = 0;
	isOnTop = false;
	isElectron = electron;
	FastLED.addLeds<NEOPIXEL, PINPIXEL>(leds, NUMPIXEL);
}

void ExperienceEffects::setup(){
  for (int i = 0; i < 60; i++){
		leds[i] = CRGB::Black;
	}
	FastLED.show();
	if (isElectron){
		motor.attach(EN_MOTOR, PWM_MOTOR, 0, 5000, SELECT_MOTOR);
		driver.attach(RST_DC_DRV, FLT_DC_DRV);
		driver.reset();
	}
}

void ExperienceEffects::start(){
	if (!hasStarted){
		timer = millis();
  	for (int i = 0; i < 60; i++){
			leds[i] = CRGB::Red;
		}
		FastLED.show();
		hasStarted = 1;
		if (isElectron){
			motor.setVelocity(1);
			motor.enable();
		}
	}
}

void ExperienceEffects::stayOnTop(){
	if (isElectron){
		motor.setVelocity(1);
		motor.enable();
		isOnTop = true;
	}
}

boolean ExperienceEffects::getOnTop(){
	return isOnTop;
}

long ExperienceEffects::getTimer(){
	return timer;
}

long ExperienceEffects::getStart(){
	return hasStarted;
}
