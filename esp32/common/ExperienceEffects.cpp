#include "ExperienceEffects.h"

/* Constuctor */
ExperienceEffects::ExperienceEffects(boolean electron)
{
	hasStarted = 0;
	timer = 0;
	isOnTop = false;
	isElectron = electron;
	FastLED.addLeds<NEOPIXEL, PINPIXEL>(leds, NUMPIXEL);
	pinMode(BUILTIN_LED, OUTPUT);
	pinMode(GO_BACK, INPUT_PULLUP);
	pinMode(GO_FORWARD, INPUT_PULLUP);
	digitalWrite(BUILTIN_LED, LOW);
	pinMode(INTERRUPT, INPUT_PULLUP);
}

void ExperienceEffects::setup()
{
    for (int i = 0; i < 60; i++)
    {
		leds[i] = CRGB::Red;    /* red (Non Connected default status) */
	}
	FastLED.show();

	if (isElectron)
    {
		motor.attach(EN_MOTOR, PWM_MOTOR, 0, 5000, SELECT_MOTOR);
		driver.attach(RST_DC_DRV, FLT_DC_DRV);
		driver.reset();
	}
}

void ExperienceEffects::goBack()
{
	motor.setVelocity(1);
	motor.enable();
}

void ExperienceEffects::goForward()
{
	motor.setVelocity(-1);
	motor.enable();
}

void ExperienceEffects::motorStop()
{
	motor.setVelocity(0);
	motor.enable();
}

void ExperienceEffects::connected()
{
    for (int i = 0; i < 60; i++)
    {
		leds[i] = CRGB::Green;
	}
	FastLED.show();
	digitalWrite(BUILTIN_LED, HIGH);
}

void ExperienceEffects::start()
{
	if (!hasStarted)
    {
		timer = millis();
        for (int i = 0; i < 60; i++)
        {
			leds[i] = CRGB::Purple;
		}
		FastLED.show();
		hasStarted = 1;
		if (isElectron)
        {
			motor.setVelocity(-1);
			motor.enable();
		}
	}
}

void ExperienceEffects::stayOnTop()
{
	if (isElectron)
    {
		motor.setVelocity(0.1);
		motor.enable();
		isOnTop = true;
	}
	for (int i = 0; i < 60; i++)
    {
		leds[i] = CRGB::Blue;
	}
	FastLED.show();
}

boolean ExperienceEffects::getOnTop()
{
	return isOnTop;
}

long ExperienceEffects::getTimer()
{
	return timer;
}

long ExperienceEffects::getStart()
{
	return hasStarted;
}
