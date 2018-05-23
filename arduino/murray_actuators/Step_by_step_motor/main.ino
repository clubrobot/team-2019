#include <Arduino.h>
#include "PIN.h"
#include "../../common/SerialTalks.h"
#include "../../common/ShiftRegister.h"
#include "../../common/StepByStepMotor.h"

ShiftRegister shift;
StepByStepMotor motor;

void setup()
{
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

	shift.attach(LATCHPIN,CLOCKPIN,DATAPIN);

	motor.attach(STEP_PAP, DIR_PAP, ENABLE_PAP, RST_PAP, SLEEP_PAP);
	motor.begin();

	motor.set_position(50);
	
}
void loop()
{
	talks.execute();
	motor.update();

}