
#include "OLED.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include <math.h>

// OLED pins
#define OLED_CLK  27
#define OLED_MOSI 26
#define OLED_RES  28
#define OLED_DC   29
#define OLED_CS   30

U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI oled(
    U8G2_R0,
    OLED_CLK,
    OLED_MOSI,
    OLED_CS,
    OLED_DC,
    OLED_RES
);

static bool initialized = false;
static unsigned long lastUpdate = 0;

// Skjermens sentrum
constexpr int CX = 64;
constexpr int CY = 32;

// Justering for montering
constexpr float PITCH_OFFSET = 90.0f;
constexpr float PITCH_SCALE = 1.5f;

static void ensureInitialized() {
    if (!initialized) {
        oled.begin();
        initialized = true;
    }
}

// Konverter vinkel til kompasskurs 0-359 grader
static float getHeading(float yaw) {
    float heading = fmodf(yaw + 360.0f, 360.0f);
    return heading;
}

// Finn himmelretning
static const char* getDirection(float heading) {
    const char* directions[] = {
        "N", "NE", "E", "SE",
        "S", "SW", "W", "NW"
    };

    int index = ((int)((heading + 22.5f) / 45.0f)) % 8;
    return directions[index];
}

void OLEDPrint(Orientation orientation) {
    ensureInitialized();

    if (millis() - lastUpdate < 100) return;
    lastUpdate = millis();

    if (!isfinite(orientation.pitch) ||
        !isfinite(orientation.roll) ||
        !isfinite(orientation.yaw)) return;

    // Korriger for skjermens montering
    float pitch = orientation.roll - PITCH_OFFSET;
    float roll = orientation.pitch;
    float heading = getHeading(orientation.yaw);

    float rollRad = roll * DEG_TO_RAD;
    float s = sinf(rollRad);
    float c = cosf(rollRad);
    float pitchOffset = pitch * PITCH_SCALE;

    oled.clearBuffer();
    oled.setDrawColor(1);

    // Kunstig horisont
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 128; x++) {

            float distance =
                (x - CX) * s +
                (y - CY) * c -
                pitchOffset;

            if (distance > 0) {
                oled.drawPixel(x, y);
            }
        }
    }

    // Flysymbol
    oled.setDrawColor(0);
    oled.drawBox(42, 30, 18, 5);
    oled.drawBox(68, 30, 18, 5);
    oled.drawBox(61, 29, 7, 7);

    oled.setDrawColor(1);
    oled.drawHLine(44, 32, 15);
    oled.drawHLine(70, 32, 15);
    oled.drawFrame(62, 30, 5, 5);

    // Toppfelt: pitch og roll
    oled.setDrawColor(0);
    oled.drawBox(0, 0, 128, 12);

    oled.setDrawColor(1);
    oled.setFont(u8g2_font_6x10_tf);

    oled.setCursor(0, 9);
    oled.print("P:");
    oled.print(pitch, 0);

    oled.setCursor(70, 9);
    oled.print("R:");
    oled.print(roll, 0);

    // Bunnfelt: kompasskurs
    oled.setDrawColor(0);
    oled.drawBox(0, 52, 128, 12);

    oled.setDrawColor(1);
    oled.setCursor(0, 62);
    oled.print("HDG:");

    oled.print(heading, 0);
    oled.print(" deg");

    oled.setCursor(108, 62);
    oled.print(getDirection(heading));

    oled.sendBuffer();
}

void OLEDerror() {
    ensureInitialized();

    if (millis() - lastUpdate < 100) return;
    lastUpdate = millis();

    oled.clearBuffer();

    oled.setFont(u8g2_font_helvB12_tf);
    oled.drawStr(34, 20, "ERROR");

    oled.drawHLine(0, 27, 128);

    oled.setFont(u8g2_font_6x10_tf);
    oled.drawStr(12, 42, "IMU DISCONNECTED");
    oled.drawStr(18, 56, "Reconnecting...");

    oled.sendBuffer();
}
