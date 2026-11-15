
#include <Arduino.h>
#include <math.h>

void setup() {
    Serial.begin(115200);
}

void loop() {
    float t = millis() / 1000.0f;

    float pitch = 30.0f * sin(t);
    float roll  = 60.0f * sin(2.0f * t);
    float yaw   = 90.0f * sin(0.5f * t);

    Serial.print(">Pitch:");
    Serial.print(pitch);
    Serial.print(",Roll:");
    Serial.print(roll);
    Serial.print(",Yaw:");
    Serial.println(yaw);

    delay(20);  // 50 målinger per sekund
}
