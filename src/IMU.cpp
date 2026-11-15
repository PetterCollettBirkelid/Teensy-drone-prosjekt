#include "IMU.h"
#include <Wire.h>
#include <Adafruit_BNO08x.h>

Adafruit_BNO08x imu(-1);

sh2_SensorValue_t sensorValue;

bool initIMU(){
    return imu.begin_I2C(0x4A, &Wire) && imu.enableReport(SH2_ROTATION_VECTOR, 10000);
}

bool readIMU(){
    return imu.getSensorEvent(&sensorValue) && sensorValue.sensorId == SH2_ROTATION_VECTOR;
}

Orientation getOrientation() {
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

    pitch = asin(2*(w*y - x*z)) * RAD_TO_DEG;
    roll = atan2(2*(w*x + y*z), 1-2*(x*x + y*y)) * RAD_TO_DEG;
    yaw = atan2(2*(w*z + x*y), 1-2*(y*y + z*z)) * RAD_TO_DEG;

    return Orientation{pitch, roll, yaw};
}