#include "instructions.h"
#include "../common/EndStop.h"
#include "../common/DCMotor.h"
#include "../common/VelocityServo.h"
#include "../common/SerialTalks.h"
#include "../common/FullSpeedServo.h"
#include "../common/Clock.h"
#include <Servo.h>
#include "PIN.h"

#define GRIP_CYLINDER_ANGLE 40

extern VelocityServo gripper;
extern FullSpeedServo dispenser;
extern EndStop highStop;
extern EndStop lowStop;
extern EndStop leftmustache;
extern EndStop rightmustache;
extern DCMotor gripperMotor;
extern bool elevatorMoving;
extern bool motorError;
extern Clock movingTime;

void WRITE_GRIP(SerialTalks &inst, Deserializer &input, Serializer &output)
{   
    
    int val = input.read<int>();
    if (val >= 0)
    {
        if (!gripper.attached())
        {
            gripper.attach(SERVO3);
        }
        gripper.write(val);
    }
    else if (val < 0)
    {
        gripper.detach();
    }
}

void OPEN_GRIP(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    int val = input.read<int>();
    if (val >= 0)
    {
        if (!gripper.attached())
        {
            gripper.attach(SERVO3);
        }
        gripper.Velocitywrite(val);
    }
}

void SET_GRIP_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output){
    gripper.setVelocity(input.read<float>());
}

void WRITE_DISPENSER(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    int val = input.read<int>();
    float t = input.read<float>();
    if (val >= 0)
    {
        if (!dispenser.attached())
        {
            dispenser.attach(SERVO2);
        }
        dispenser.SpeedWrite(val,t);
    }
    else if (val < 0)
    {
        dispenser.detach();
    }
}

void IS_UP(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<bool>(highStop.getState());
    output.write<bool>(motorError);
}

void IS_DOWN(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<bool>(lowStop.getState());
    output.write<bool>(motorError);
}

void SET_MOTOR_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    float vel = input.read<float>();
    bool ok = false;
    if (gripper.attached())
    {
        if(gripper.read() <= 5){
            gripperMotor.setVelocity(vel);
            ok = true;
        }
    }
    else{
        gripperMotor.setVelocity(vel);
        ok = true;
    }
    if(ok){
        if(vel != 0){
            elevatorMoving = true;
            movingTime.restart();
            motorError = false;
        }
        if (vel > 0){
            gripper.detach();
        }
        if(vel == 0){
            elevatorMoving = false;
        }
    }
    output.write<bool>(ok);
    
}

void GET_MOTOR_VELOCITY(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<float>(gripperMotor.getVelocity());
}

void GET_LEFT_MUSTACHE(SerialTalks &inst, Deserializer &input, Serializer &output){
    output.write<bool>(leftmustache.getState());
}

void GET_RIGHT_MUSTACHE(SerialTalks &inst, Deserializer &input, Serializer &output){
    output.write<bool>(rightmustache.getState());
}