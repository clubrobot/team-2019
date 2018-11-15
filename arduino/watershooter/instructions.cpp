#include "instructions.h"
#include <Servo.h>
#include "PIN.h"
#include "BallsShaker.h"
#include "../common/SerialTalks.h"
#include "../common/BrushlessMotor.h"
#include "../common/Adafruit_TCS34725.h"
#include "../common/Wire.h"

extern BrushlessMotor motor;
extern Servo indoor;
extern Servo outdoor;
extern Servo trash;
extern BallsShaker shaker;
extern Servo trashUnloader;
extern Adafruit_TCS34725 waterSensor;
extern Servo beeActivator;
extern int ballCount;
uint16_t red, green, blue, clear;


void WRITE_INDOOR(SerialTalks &inst, Deserializer &input, Serializer &output){
	indoor.write(input.read<int>());
}

void GET_INDOOR(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<int>(indoor.read());
}

void WRITE_OUTDOOR(SerialTalks &inst, Deserializer &input, Serializer &output){
	outdoor.write(input.read<int>());
}

void GET_OUTDOOR(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<int>(outdoor.read());
}

void WRITE_TRASH(SerialTalks &inst, Deserializer &input, Serializer &output){
	trash.write(input.read<int>());
}

void GET_TRASH(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<int>(trash.read());
}


void WRITE_SHAKER_VERTICAL(SerialTalks &inst, Deserializer &input, Serializer &output){
	shaker.writeVertical(input.read<int>());
}

void GET_SHAKER_VERTICAL(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<int>(shaker.getVertical());
}

void WRITE_SHAKER_HORIZONTAL(SerialTalks &inst, Deserializer &input, Serializer &output){
	shaker.writeHorizontal(input.read<int>());
}

void GET_SHAKER_HORIZONTAL(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<int>(shaker.getHorizontal());
}

void WRITE_TRASH_UNLOADER(SerialTalks &inst, Deserializer &input, Serializer &output){
	trashUnloader.write(input.read<int>());
}
void GET_TRASH_UNLOADER(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<int>(trashUnloader.read());
}

void SET_MOTOR_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<int>(motor.setVelocity(input.read<int>()));
}

void GET_MOTOR_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<int>(motor.getVelocity());
}

void GET_WATER_COLOR(SerialTalks &inst, Deserializer &input, Serializer &output){
	waterSensor.setInterrupt(false);
    waterSensor.getRawData(&red, &green, &blue, &clear);
	output.write<int>(red);
	output.write<int>(green);
	output.write<int>(blue);
}

void SET_MOTOR_PULSEWIDTH(SerialTalks &inst, Deserializer &input, Serializer &output) {
	output.write<int>(motor.setPulsewidth(input.read<int>()));
}

void GET_MOTOR_PULSEWIDTH(SerialTalks &inst, Deserializer &input, Serializer &output) {
	int pulsewidth = motor.readMicroseconds();
	output.write<int>(pulsewidth);
}

void FORCE_PULSEWIDTH(SerialTalks &inst, Deserializer &input, Serializer &output){
	motor.forcePulsewidth(input.read<int>());
}

void SET_LED_ON(SerialTalks& inst, Deserializer& input, Serializer& output){
	digitalWrite(LED, HIGH);
}

void SET_LED_OFF(SerialTalks& inst, Deserializer& input, Serializer& output){
	digitalWrite(LED, LOW);
}

void ENABLE_SHAKING_DIFF(SerialTalks &inst, Deserializer &input, Serializer &output){
	shaker.enableShakerDiffFreq();
}

void ENABLE_SHAKING_EQUAL(SerialTalks &inst, Deserializer &input, Serializer &output){
	shaker.enableShakerEqualFreq();
}

void DISABLE_SHAKING(SerialTalks &inst, Deserializer &input, Serializer &output){
	shaker.disableShaker();
}

void SET_SHAKER_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	shaker.set_velocity(input.read<int>());
}

void GET_EMERGENCY_STATE(SerialTalks& inst, Deserializer& input, Serializer& output){
	output.write<int>(digitalRead(SWITCH1));
}

void DISABLE(SerialTalks &inst, Deserializer &input, Serializer &output){
	shaker.disableShaker();
	motor.setPulsewidth(1000);
}

void WRITE_BEEACTIVATOR(SerialTalks& inst, Deserializer& input, Serializer& output){
	int val = input.read<int>();
    if (val >= 0)
    {
        if (!beeActivator.attached())
        {
            beeActivator.attach(SWITCH2);
        }
        beeActivator.write(val);
    }
    else if (val < 0)
    {
        beeActivator.detach();
    }
}

void GET_LAUNCHED_WATER(SerialTalks& inst, Deserializer& input, Serializer& output){
	output.write<int>(ballCount);
	ballCount = 0;
}
