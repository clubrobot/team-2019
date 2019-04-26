#include <Arduino.h>
#include "Arduino.h"
#include "../common/ExperienceEffects.h"
#include "../common/mqtt/Adafruit_MQTT.h"
#include "../common/mqtt/Adafruit_MQTT_Client.h"
#include <WiFi.h>

#define WLAN_SSID       "CLUB_ROBOT"
#define WLAN_PASS       "zigouigoui"

#define AIO_SERVER      "192.168.1.13"
#define AIO_SERVERPORT  1883

#define TEMPS_MONTEE    20000

ExperienceEffects experience(true);

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);

Adafruit_MQTT_Subscribe start = Adafruit_MQTT_Subscribe(&mqtt, "topic/start");
Adafruit_MQTT_Publish start_publish = Adafruit_MQTT_Publish(&mqtt, "topic/start");
Adafruit_MQTT_Publish isOnTop = Adafruit_MQTT_Publish(&mqtt, "topic/isOnTop");

bool MQTT_connect();

bool mqtt_connected = false;

void setup()
{
  Serial.begin(115200);
  experience.setup();
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    if (digitalRead(GO_BACK)==LOW){
      experience.goBack();
    }

    if (digitalRead(GO_FORWARD)==LOW){
      experience.goForward();
    }

    if (digitalRead(GO_FORWARD) == HIGH && digitalRead(GO_BACK) == HIGH){
      experience.motorStop();
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  digitalWrite(16, LOW);
  mqtt.subscribe(&start);

  if(MQTT_connect())
  {
    mqtt_connected = true;
    experience.connected();
  }
  else
  {
    mqtt_connected = false;
  }
  
}

void loop()
{
  static long starttime = millis();
  static long currenttime = 0;

  if(!mqtt_connected)
  {
    currenttime = millis();  
    if (currenttime - starttime >= 5000)
    {
      if(MQTT_connect())
      {
        mqtt_connected = true;
        experience.connected();
      }
      else
      {
        mqtt_connected = false;
      }
      starttime = currenttime;
    }
  }

  if (digitalRead(GO_BACK)==LOW && experience.getStart() == 0){
    experience.goBack();
  }

  if (digitalRead(GO_FORWARD)==LOW && experience.getStart() == 0){
    experience.goForward();
  }

  if (digitalRead(GO_FORWARD) != LOW && digitalRead(GO_BACK) != LOW &&  experience.getStart() == 0){
    experience.motorStop();
  }

  
  if(mqtt_connected)
  {
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(100))) {
      if (subscription == &start) {
        Serial.print(F("Got: "));
        Serial.println((char *)start.lastread);
        char *value = (char *)start.lastread;
        int current = atoi(value);
        if (current == 2){
          Serial.println("starting");
          experience.start();
        }
      }
    }

    if (experience.getTimer()+TEMPS_MONTEE < millis() && experience.isElectron && experience.getStart() == 1){
      experience.stayOnTop();
      isOnTop.publish("1");
    }
  }

  delay(100);
}

bool MQTT_connect() 
{
  int8_t ret;

  if (mqtt.connected()) 
  {
    experience.connected();
    return true;
  }

  Serial.print("Connecting to MQTT... ");

  if((ret = mqtt.connect()) != 0) 
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    return false;
  }
  else
  {
    return true;
  }
  

  Serial.println("MQTT Connected!");
}
