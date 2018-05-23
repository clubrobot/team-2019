#ifndef __ENDSTOP_H__
#define __ENDSTOP_H__


class EndStop {

    private :
    
    int m_pin;

    public : 

    bool getState();
    void attach(int pin);
    void detach();
};

#endif // __ENDSTOP_H__
