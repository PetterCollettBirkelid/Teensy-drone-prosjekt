
#include <Arduino.h>
#include <math.h>
#include "IMU.h"
#include "OLED.h"

Orientation orientation;

void setup() {
    delay(1000);
    Serial.begin(115200);
    initIMU();
}

void loop() {
    if (IMUconnected()) {
        if (readIMU()){
            getOrientation(orientation);
            Serial.printf(">Pitch:%.3f,Roll:%.3f,Yaw:%.3f\r\n", orientation.pitch, orientation.roll, orientation.yaw);
        }
        OLEDPrint(orientation);
    } else {
        recoverIMU();
        OLEDerror();
    }
}