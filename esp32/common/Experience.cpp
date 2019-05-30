#include "Experience.h"


Experience::Experience(bool isElectron)
{
    _mutex.acquire();

    _isElectron = isElectron;
    _isStarted  = false;
    _isOnTop    = false;

    _motorForward   = false;
    _motorBackward  = false;
    _motorStop      = false;

    _time = 0;

    FastLED.addLeds<NEOPIXEL, PINPIXEL>(_leds, NUMPIXEL);

    pinMode(GO_BACK,    INPUT_PULLUP);
	pinMode(GO_FORWARD, INPUT_PULLUP);
	pinMode(INTERRUPT,  INPUT_PULLUP);

	if (_isElectron)
    {
		_motor.attach(EN_MOTOR, PWM_MOTOR, 0, 5000, SELECT_MOTOR);
		_driver.attach(RST_DC_DRV, FLT_DC_DRV);
		_driver.reset();
	}

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

    updateAnimation();
    manageMotorSwitchs();

    if (_isStarted && (_time + (TEMPS_MIN * 1000) < millis()) && _isElectron)
    {
        stayOnTop();
        _isStarted = false;
    }

    _mutex.release();
}

void Experience::start(void)
{
    _mutex.acquire();
    if (!_isStarted)
    {
        _time = millis();
        _isStarted = true;
		
        for (int i = 0; i < 8; i++)
        {
			_leds[i] = CRGB::OrangeRed;
		}
		
		if (_isElectron)
        {
			_motor.setVelocity(-0.4);
			_motor.enable();
		}
	}
    _mutex.release();
}

void Experience::stayOnTop(void)
{
    _mutex.acquire();
    if (_isElectron)
    {
		_motor.setVelocity(0.1);
		_motor.enable();
		_isOnTop = true;
	}
	for (int i = 0; i < 8; i++)
    {
		_leds[i] = CRGB::Orchid;
	}
    _mutex.release();
}

void Experience::connected(void)
{
    _mutex.acquire();
    for (int i = 0; i < 1; i++)
    {
        _leds[i] = CRGB::Blue;
    }
    _mutex.release();
}

void Experience::updateAnimation(void)
{
    FastLED.show();
}

void Experience::manageMotorSwitchs(void)
{
    if(!_isStarted)
    {
        if((digitalRead(GO_BACK) == LOW) && !_motorBackward)
        {
            goBack();
        }
        else if ((digitalRead(GO_FORWARD) == LOW) && !_motorForward)
        {
            goForward();
        }
        else if ((digitalRead(GO_BACK) == HIGH) && (digitalRead(GO_FORWARD) == HIGH) && !_motorStop)
        {
            motorStop();
        }
    }
}

void Experience::goBack(void)
{
    _motor.setVelocity(0.4);
	_motor.enable();

    _motorForward   = false;
    _motorBackward  = true;
    _motorStop      = false;

}

void Experience::goForward(void)
{
    _motor.setVelocity(-0.4);
	_motor.enable();

    _motorForward   = true;
    _motorBackward  = false;
    _motorStop      = false;
}

void Experience::motorStop(void)
{
    _motor.setVelocity(0);
	_motor.enable();

    _motorForward   = false;
    _motorBackward  = false;
    _motorStop      = true;
}

