#include "Experience.h"


Experience::Experience()
{
    _mutex.acquire();

    _isStarted  = false;
    _isOnTop    = false;

    FastLED.addLeds<NEOPIXEL, PINPIXEL>(_leds, NUMPIXEL);

	_motor.attach(EN_MOTOR, PWM_MOTOR, 0, 5000, SELECT_MOTOR);
	_driver.attach(RST_DC_DRV, FLT_DC_DRV);
	_driver.reset();

    for (int i = 0; i < 1; i++)
    {
		_leds[i] = CRGB::Red;    /* red (Non Connected default status) */
	}

    for (int i = 1; i < 8; i++)
    {
		_leds[i] = CRGB::Black;
	}
    _mutex.release();
}

void Experience::process(float timestep)
{
    _mutex.acquire();

    if (_isStarted && !_isOnTop && (_clock.getElapsedTime() > TEMPS_MIN))
    {
        stayOnTop();
    }

    _mutex.release();
}

void Experience::start(void)
{
    _mutex.acquire();
    if (!_isStarted)
    {
        _clock.restart();
        _isStarted = true;
		
        for (int i = 0; i < 8; i++)
        {
			_leds[i] = CRGB::OrangeRed;
		}
		FastLED.show();
        
		_motor.setVelocity(RUN_CONSTANT);
		_motor.enable();
	}
    _mutex.release();
}

void Experience::stayOnTop(void)
{
	_motor.setVelocity(STOP_CONSTANT);
	_isOnTop = true;

}

void Experience::connected(void)
{
    _mutex.acquire();
    for (int i = 0; i < 1; i++)
    {
        _leds[i] = CRGB::Blue;
    }
    FastLED.show();
    _mutex.release();
}