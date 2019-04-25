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

ExperienceEffects experience(false);

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);

Adafruit_MQTT_Subscribe start = Adafruit_MQTT_Subscribe(&mqtt, "topic/start");
Adafruit_MQTT_Publish start_publish = Adafruit_MQTT_Publish(&mqtt, "topic/start");
Adafruit_MQTT_Publish isOnTop = Adafruit_MQTT_Publish(&mqtt, "topic/isOnTop");

void MQTT_connect();

long current_time = 0;
long last_time = 0;

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
}

void loop()
{

  if (digitalRead(GO_BACK)==LOW && experience.getStart() == 0){
    experience.goBack();
  }

  if (digitalRead(GO_FORWARD)==LOW && experience.getStart() == 0){
    experience.goForward();
  }

  if (digitalRead(GO_FORWARD) != LOW && digitalRead(GO_BACK) != LOW &&  experience.getStart() == 0){
    experience.motorStop();
  }

  MQTT_connect();

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

  delay(500);
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    experience.connected();
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
