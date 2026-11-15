
#include <Arduino.h>
#include <math.h>
#include "IMU.h"

Orientation orientation;
bool imuInitialized = false;
unsigned long lastIMURead = 0;

void setup() {
    delay(1000);
    Serial.begin(115200);
}

void loop() {

    if (!imuInitialized) {
        imuInitialized = initIMU();
        if (!imuInitialized){
            delay(100);
        }
        lastIMURead = millis();
    } else if (millis() - lastIMURead >= 1000) {
        Serial.println("Lost connection to IMU...");
        imuInitialized = false;
        delay(100);
    } else if (readIMU()) {
        lastIMURead = millis();
        orientation = getOrientation();
        Serial.printf(">Pitch:%.3f,Roll:%.3f,Yaw:%.3f\r\n", orientation.pitch, orientation.roll, orientation.yaw);
    }
}
