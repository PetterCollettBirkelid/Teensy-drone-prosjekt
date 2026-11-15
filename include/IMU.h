struct Orientation {
    float pitch;
    float roll;
    float yaw;
};

bool initIMU();

bool readIMU();

Orientation getOrientation();