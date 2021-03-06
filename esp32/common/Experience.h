#include <Arduino.h>
#include "../common/fastLED/FastLED.h"
#include "../common/ESP32-Arduino-Servo-Library/src/Servo.h"
#include "../common/DCMotor.h"
#include "thread_tools.h"

#define NUMPIXEL 			10
#define PINPIXEL 			33

#define PWM_MOTOR 		    21
#define PWMCHANNEL 		    0
#define EN_MOTOR 			27
#define SELECT_MOTOR 	    12
#define RST_DC_DRV 		    23
#define FLT_DC_DRV		    0

#define BUILTIN_LED		    2

#define SERVO1 				32
#define SERVO2 				25

#define ROTARY 				4

#define INTERRUPT			4

#define TEMPS_MIN			8

#define GO_BACK				32
#define GO_FORWARD		    25

#define STOP_CONSTANT       0.0
#define RUN_CONSTANT        0.6

class Experience : public PeriodicProcess
{
    public:
        Experience();
        
        void start(void);
        void connected(void);

        void stayOnTop(void);

        bool isStarted(void) const {return _isStarted;}
    
    protected:
        virtual void process(float timestep);

    private:

        bool _isStarted;
        bool _isOnTop;

        DCMotor _motor;
		DCMotorsDriver _driver;

        CRGB _leds[NUMPIXEL];

        Clock _clock;

        Mutex _mutex;

};
