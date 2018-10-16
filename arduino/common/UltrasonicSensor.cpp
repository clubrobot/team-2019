#include "UltrasonicSensor.h"
#include <Arduino.h>
#define TEMPORISATION 50000


EchoHandler echohandler2(2);
EchoHandler echohandler3(3);

TrigHandler trighandler1(1);
TrigHandler trighandler2(2);
TrigHandler trighandler3(3);
TrigHandler trighandler4(4);
TrigHandler trighandler5(5);
TrigHandler trighandler6(6);
TrigHandler trighandler7(7);
TrigHandler trighandler8(8);
TrigHandler trighandler9(9);

EchoHandler::EchoHandler(int echo)
  : m_echoP(echo) {
  m_enable = 0;
  m_startTime = 0;
  m_endTime = 0;
}

unsigned long EchoHandler::getEndTime() {
  return m_endTime;
}

unsigned long EchoHandler::getStartTime() {
  return m_startTime;
}

bool EchoHandler::getflag() {
  return m_flag;
}

int EchoHandler::getPin() {
  return m_echoP;
}

void EchoHandler::enabler() {
    m_enable +=1;
    pinMode(m_echoP, INPUT);
    if (m_echoP == 2) {
      attachInterrupt(digitalPinToInterrupt(2), EchoHandler::echoInterrupt_2, CHANGE);
    }
    if (m_echoP == 3) {
      attachInterrupt(digitalPinToInterrupt(3), EchoHandler::echoInterrupt_3, CHANGE);
  }
}

void EchoHandler::echoInterrupt_2() {
  if (digitalRead(2) == HIGH)
  {
    echohandler2.m_startTime = micros();
  }
  else if (digitalRead(2) == LOW)
  {
    echohandler2.m_endTime = micros();
    echohandler2.m_flag = true;
  }
}

void EchoHandler::echoInterrupt_3() {
  if (digitalRead(3) == HIGH)
  {
    echohandler3.m_startTime = micros();
  }
  else if (digitalRead(3) == LOW)
  {
    echohandler3.m_endTime = micros();
    echohandler3.m_flag = true;
  }
}

void EchoHandler::setflag(bool b){
  m_flag = b;
}

TrigHandler::TrigHandler(int Trig)
  : m_trigP(Trig)
{
  m_enable = 0;
  m_nReady = 0;
  m_ready = true;
}

void TrigHandler::trig() {
  if (m_ready) {
    digitalWrite(m_trigP, HIGH);
    delayMicroseconds(20);
    digitalWrite(m_trigP, LOW);
    m_ready = false;
    m_nReady = 0;
  }
}

void TrigHandler::enabler() {
  m_enable += 1;
  pinMode(m_trigP, OUTPUT);
}

void TrigHandler::disabler() {
  if (m_enable != 0) {
    m_enable -= 1;
  }
}


void TrigHandler::Ready() {
  m_nReady += 1;
  if (m_nReady >= m_enable) {
    m_ready = true;
  }
}

bool TrigHandler::getReady() {
  return m_ready;
}


bool UltrasonicSensor::getReady() {
  return trighandler->getReady();
}

void UltrasonicSensor::trig() {
  trighandler->trig();
}

void UltrasonicSensor::update() {
  if (echohandler->getflag()) {
    m_mesure = (echohandler->getEndTime() - echohandler->getStartTime()) * 340 / 2000;
    filtrage();
    echohandler->setflag(false);
    m_dejaTest = false;
  }

  else if (((micros() - echohandler->getEndTime()) >= TEMPORISATION) && !m_dejaTest) {
    m_dejaTest = true;
    trighandler->Ready();
  }
}

void UltrasonicSensor::filtrage() {
  m_inf2 = (m_mesure < 26);
  if (m_mesure > 50) {
    m_inf2 = false;
  }
}


unsigned int UltrasonicSensor::getMesure() {
  if (m_inf2) {
    return 25;
  }
  else {
    return m_mesure;
  }
}


void UltrasonicSensor::attach(int trig, int echo)
{
  if (echo == 2) {
    echohandler = &echohandler2;
  }
  else if (echo == 3) {
    echohandler = &echohandler3;
  }
  if (trig == 1) {
    trighandler = &trighandler1;
  }
  if (trig == 2) {
    trighandler = &trighandler2;
  }
  if (trig == 3) {
    trighandler = &trighandler3;
  }
  if (trig == 4) {
    trighandler = &trighandler4;
  }
  if (trig == 5) {
    trighandler = &trighandler5;
  }
  if (trig == 6) {
    trighandler = &trighandler8;
  }
  if (trig == 7) {
    trighandler = &trighandler7;
  }
  if (trig == 8) {
    trighandler = &trighandler8;
  }
  if (trig == 9) {
    trighandler = &trighandler9;
  }
  echohandler->enabler();
  trighandler->enabler();
}