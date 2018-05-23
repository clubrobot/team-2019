#ifndef __ULTRASONICSENSOR_H__
#define __ULTRASONICSENSOR_H__

class TrigHandler {

  private :
   
   int m_enable;
   int m_nReady;
   int m_trigP;
   bool m_ready;
   

  public : 
    
  TrigHandler(int Trig);
  void enabler();
  void Ready();
  bool getReady();
  void trig();
  void disabler();

};

class EchoHandler {

  private :

    int m_echoP;
    unsigned long m_startTime;
    unsigned long m_endTime;
    bool m_flag;
    static void echoInterrupt_2();
    static void echoInterrupt_3(); 
    int m_enable; 

  public :

    EchoHandler(int echo);
    unsigned long getEndTime();
    unsigned long getStartTime();
    bool getflag();
    int getPin();
    void enabler();
    void setflag(bool b);
};


class UltrasonicSensor {
  private :
    unsigned int m_mesure;
    EchoHandler* echohandler;
    TrigHandler* trighandler;
    bool m_dejaTest;
    void filtrage();
    bool m_inf2;

  public :
    void attach(int trig, int echo);
    void update();
    unsigned int getMesure();
    void trig();
    bool getReady();
};

extern EchoHandler echohandler2;
extern EchoHandler echohandler3;

extern TrigHandler trighandler1;
extern TrigHandler trighandler2;
extern TrigHandler trighandler3;
extern TrigHandler trighandler4;
extern TrigHandler trighandler5;
extern TrigHandler trighandler6;
extern TrigHandler trighandler7;
extern TrigHandler trighandler8;
extern TrigHandler trighandler9;

#endif