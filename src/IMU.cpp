#include "IMU.h"
#include <Wire.h>
#include <Adafruit_BNO08x.h>

Adafruit_BNO08x imu(-1);
sh2_SensorValue_t sensorValue;

unsigned long lastIMURead = 0;
unsigned long lastIMURecover = 0;
bool imuInitialized = false;

void initIMU(){
    if(imu.begin_I2C(0x4A, &Wire) && imu.enableReport(SH2_ROTATION_VECTOR, 10000)){
        imuInitialized = true;
        lastIMURead = millis();
    } else {
        imuInitialized = false;
    }
}

bool readIMU(){
    if (imu.getSensorEvent(&sensorValue) && sensorValue.sensorId == SH2_ROTATION_VECTOR){
        lastIMURead = millis();
        return true;
    }
    return false;
}

bool IMUconnected(){
    return (millis() - lastIMURead) < 500 && imuInitialized;
}

void recoverIMU(){
    if (millis() - lastIMURecover > 1000){
        lastIMURecover = millis();
        initIMU();
    }
}

void getOrientation(Orientation& orientation){
    float w;
    float x;
    float y;
    float z;

    w = sensorValue.un.rotationVector.real;
    x = sensorValue.un.rotationVector.i;
    y = sensorValue.un.rotationVector.j;
    z = sensorValue.un.rotationVector.k;

    float pitch;
    float roll;
    float yaw;

    float sinPitch = constrain(2*(w*y - x*z), -1.0f, 1.0f);
    pitch = asin(sinPitch) * RAD_TO_DEG;
    roll = atan2(2*(w*x + y*z), 1-2*(x*x + y*y)) * RAD_TO_DEG;
    yaw = atan2(2*(w*z + x*y), 1-2*(y*y + z*z)) * RAD_TO_DEG;

    orientation.pitch = pitch;
    orientation.roll = roll;
    orientation.yaw = yaw;
}