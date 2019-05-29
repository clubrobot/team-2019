#include <Wire.h>
#include <VL6180X.h>
/**
 * VL6180X Calibration Program
 * Connect to Capteur1
 */

#define XSHUT_VL53_1 10
#define XSHUT_VL61_1 11

#define XSHUT_VL53_2 3
#define XSHUT_VL61_2 4

VL6180X sensor = VL6180X();

void enableSensor(uint8_t shutpin, bool mode);


void setup() {
    Wire.begin();
    Serial.begin(115200);
    Serial.println("VL6180X Calibration Program");
    //Shutdown every Sensors apart from the VL61 on Capteur1
    enableSensor(XSHUT_VL61_2, false);
    enableSensor(XSHUT_VL53_2, false);
    enableSensor(XSHUT_VL53_1, false);

    sensor.init();
    sensor.configureDefault();
    sensor.setTimeout(40);
    sensor.setScaling(1);
    sensor.writeReg(VL6180X::READOUT__AVERAGING_SAMPLE_PERIOD, 0xF0);
}

void loop() {
    float average = 0;
    int range = 0;
    int N = 0;
    int offset = 0;
    Serial.println("1 - Get a Single Value");
    Serial.println("2 - Get 10 Values with an Average");
    Serial.println("3 - Get N Values with an Average");
    Serial.println("4 - Start Continuous Mode");
    Serial.println("5 - Set New Offset");
    Serial.println("Choose an Option :");
    while(!Serial.available());
    int option = Serial.parseInt();
    switch(option){
        case 1:
            Serial.print("Distance : ");
            Serial.print(sensor.readRangeSingleMillimeters());
            Serial.println("mm");
            return;
        case 2:
            Serial.print("Reading 10 Values :");
            for(int i=1; i<11;i++){
                Serial.print("Value ");
                Serial.print(i);
                Serial.print(" : ");
                range = sensor.readRangeSingleMillimeters();
                average += range;
                Serial.print(range);
                Serial.println("mm");
            }
            average /= 10.0;
            Serial.println("Average : ");
            Serial.print(average);
            Serial.println("mm");
            return;
        case 3:
            Serial.print("Choose Number of Values :");
            while(!Serial.available());
            N = Serial.parseInt();
            Serial.println(N);
            for(int i=1; i<N+1;i++){
                Serial.print("Value ");
                Serial.print(i);
                Serial.print(" : ");
                range = sensor.readRangeSingleMillimeters();
                average += range;
                Serial.print(range);
                Serial.println("mm");
            }
            average /= (float)N;
            Serial.println("Average : ");
            Serial.print(average);
            Serial.println("mm");
            return;
        case 4:
            sensor.startRangeContinuous(10);
            while(1){
                Serial.print("Value : ");
                Serial.print(sensor.readRangeContinuousMillimeters());
                Serial.print("mm - Error Code : ");
                Serial.println(sensor.readReg(VL6180X::RESULT__RANGE_STATUS) >> 4);
                if(Serial.available()){
                    sensor.stopContinuous();
                    while(Serial.available() > 0){
                        Serial.read();
                    }
                    return;
                }
            }
            return;
        case 5:
            Serial.print("Current Value of Offset : ");
            Serial.println(sensor.readReg(VL6180X::SYSRANGE__PART_TO_PART_RANGE_OFFSET));
            Serial.print("Value of New offset : ");
            while(!Serial.available());
            offset = Serial.parseInt();
            Serial.println(offset);
            sensor.writeReg(VL6180X::SYSRANGE__PART_TO_PART_RANGE_OFFSET,offset);
            return;
        default:
            return;
    }
}

void enableSensor(uint8_t shutpin, bool mode)
{
    if(mode){
        pinMode(shutpin, INPUT);
        delay(2);               // Not ideal but we need to wait for the sensors to boot
    } else {
        pinMode(shutpin, OUTPUT);
        digitalWrite(shutpin, LOW);
    }
}
