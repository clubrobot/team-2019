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

#define TEMPS_MIN			30

#define GO_BACK				32
#define GO_FORWARD		    25


class Experience : public PeriodicProcess
{
    public:
        Experience(bool isElectron);
        
        void start(void);
        void connected(void);

        void updateAnimation(void);

        void stayOnTop(void);

        bool isStarted(void) const {return _isStarted;}
    
    protected:
        virtual void process(float timestep);

    private:

        void manageMotorSwitchs(void);
        
        void goBack(void);
		void goForward(void);
		void motorStop(void);

        bool _isElectron;
        bool _isStarted;
        bool _isOnTop;

        int _time;

        DCMotor _motor;
		DCMotorsDriver _driver;

        bool _motorForward;
        bool _motorBackward;
        bool _motorStop;

        CRGB _leds[NUMPIXEL];

        Mutex _mutex;

};
