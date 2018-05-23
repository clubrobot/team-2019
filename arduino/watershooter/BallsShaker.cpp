#include "BallsShaker.h"


void BallsShaker::attachVertical(int pin)
{
    shakerVertical.attach(pin);
}

void BallsShaker::attachHorizontal(int pin)
{
    shakerHorizontal.attach(pin);
}

void BallsShaker::detach()
{
    shakerVertical.detach();
    shakerHorizontal.detach();
}

void BallsShaker::writeHorizontal(int angle)
{
    shakerHorizontal.write(angle);
}

void BallsShaker::writeVertical(int angle)
{
    shakerVertical.write(angle);
}

int BallsShaker::getVertical()
{
    return shakerVertical.read();
}

int BallsShaker::getHorizontal()
{
    return shakerHorizontal.read();
}

void BallsShaker::shake()
{
    if(mode == BallsShaker::DIFF_FREQ)
    {
        if(count_clock_horizontal == PER_HORIZONTAL/2){
            shakerHorizontal.Velocitywrite(SHAKER_HORIZONTAL_1);
        }
        
        if(count_clock_horizontal == PER_HORIZONTAL){
            shakerHorizontal.Velocitywrite(SHAKER_HORIZONTAL_2);
            count_clock_horizontal = 0;
        }

        if(count_clock_vertical == PER_VERTICAL/2)
        {
            shakerVertical.Velocitywrite(SHAKER_VERTICAL_1);
        }
        if(count_clock_vertical == PER_VERTICAL){
            shakerVertical.Velocitywrite(SHAKER_VERTICAL_2);
            count_clock_vertical = 0;
        }
    }

    else if(mode == BallsShaker::EQUAL_FREQ)
    {
        if(count_clock_horizontal == PER_EQUAL/4){
            shakerHorizontal.Velocitywrite(SHAKER_HORIZONTAL_2);
        }

        if(count_clock_horizontal == PER_EQUAL/2){
            shakerHorizontal.Velocitywrite(SHAKER_HORIZONTAL_1);
        }

        if(count_clock_horizontal == 3*PER_EQUAL/4){
            shakerVertical.Velocitywrite(SHAKER_VERTICAL_2);
        }

        if(count_clock_horizontal == PER_EQUAL){
            shakerVertical.Velocitywrite(SHAKER_VERTICAL_1);
            count_clock_horizontal = 0;
        }
    }
    count_clock_horizontal++;
    count_clock_vertical++;
}

void BallsShaker::enableShakerEqualFreq(){
    this->mode = BallsShaker::EQUAL_FREQ;
    shakerHorizontal.Velocitywrite(SHAKER_HORIZONTAL_1);
    shakerVertical.Velocitywrite(SHAKER_VERTICAL_1);
    shakerHorizontal.enable();
    shakerVertical.enable();
    enable();
}

void BallsShaker::enableShakerDiffFreq(){
    this->mode = BallsShaker::DIFF_FREQ;
    shakerHorizontal.Velocitywrite(SHAKER_HORIZONTAL_1);
    shakerVertical.Velocitywrite(SHAKER_VERTICAL_1);
    shakerHorizontal.enable();
    shakerVertical.enable();
    enable();
}

void BallsShaker::disableShaker(){
    this->writeHorizontal(SHAKER_HORIZONTAL_1);
    this->writeVertical(SHAKER_VERTICAL_1);
    shakerHorizontal.disable();
    shakerVertical.disable();
    disable();
}

void BallsShaker::updateShaker(){
	PeriodicProcess::update();
    shakerHorizontal.update();
    shakerVertical.update();
}

void BallsShaker::set_velocity(int vel)
{
    disableShaker();
    shakerVertical.setVelocity(vel);
    shakerHorizontal.setVelocity(vel);
}

void BallsShaker::process(float timestep){
    this->shake();
}
