#include <Arduino.h>
#include "../common/SerialTalks.h"


#define SWITCH_OPCODE 0x12
#define AUTRE_OPCODE 0x11
bool enable;


void receive(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    digitalWrite(LED_BUILTIN, LOW);
    if (input.read<float>()!=1.2445000410079956) enable = false;
    if (input.read<long>()!=(long) 123456789) enable = false;
    if (input.read<float>()!= 3.141592025756836) enable = false;

    if(enable) digitalWrite(LED_BUILTIN, HIGH);

    

    
}

void autre(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    return ;
}


void setup() {
    enable = true;
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);
    talks.bind(SWITCH_OPCODE, receive);
    talks.bind(AUTRE_OPCODE, autre);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}




void loop() {
  talks.execute(); 

}
