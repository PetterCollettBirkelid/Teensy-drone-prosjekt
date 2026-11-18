#pragma once

struct Orientation {
    float pitch;
    float roll;
    float yaw;
};

void initIMU();
bool readIMU();
bool IMUconnected();
void getOrientation(Orientation& orientation);
void recoverIMU();