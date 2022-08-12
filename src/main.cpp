
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Badge2020_Accelerometer.h>
#include <Badge2020_TFT.h>

Badge2020_Accelerometer accelerometer;
Badge2020_TFT tft;

double previousunitx, previousunity;

long currentMillis;
long eyeTimeOut = 200;
long lastEyeMovement = 0;
long blinkTimeout = 5000;
long lastBlink = 0;
long pixelInterval = 10;
long previousPixelInterval = 0;

#define NEOPIXEL_PIN 2
#define NEOPIXEL_COUNT 5
long firstPixelHue = 0;

Adafruit_NeoPixel strip(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void moveEye() {
    if (currentMillis > lastEyeMovement + eyeTimeOut) {
        lastEyeMovement = currentMillis;
        int16_t x, y, z;
        accelerometer.readXYZ(x, y, z);
        accelerometer.mgScale(x, y, z);

        // make it a unit length vector
        double length = sqrt(x * x + y * y + z * z);
        double unitx = x / length;
        double unity = y / length;

        // render the eye
        if ((previousunitx != unitx) || (previousunity != unity)) {
            tft.fillCircle(60 - previousunitx * 35, 120 - previousunity * 35, 20, ST77XX_WHITE);
            tft.fillCircle(60 - unitx * 35, 120 - unity * 35, 20, ST77XX_GREEN);
            tft.fillCircle(60 - unitx * 35, 120 - unity * 35, 10, ST77XX_BLACK);
            tft.fillCircle(180 - previousunitx * 35, 120 - previousunity * 35, 20, ST77XX_WHITE);
            tft.fillCircle(180 - unitx * 35, 120 - unity * 35, 20, ST77XX_GREEN);
            tft.fillCircle(180 - unitx * 35, 120 - unity * 35, 10, ST77XX_BLACK);
            previousunitx = unitx;
            previousunity = unity;
        }
    }
}

void blinkEye() {
    if (currentMillis > lastBlink + blinkTimeout) {
        lastBlink = currentMillis - (rand() % 10000);
        tft.fillScreen(0x2c45);
        delay(100);
        tft.fillCircle(120, 120, 100, ST77XX_WHITE);
        previousunitx = 0;
        previousunity = 0;
    }
}

uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow() {
    if (currentMillis > pixelInterval + previousPixelInterval) {
        previousPixelInterval = millis();
        firstPixelHue += 256;
        if (firstPixelHue >= 5 * 65536) {
            firstPixelHue = 0;
        }

        strip.rainbow(firstPixelHue);
        strip.show();
    }
}

void setup(void) {
    Wire.begin();
    Serial.begin(115200);
    while (!Serial)
        ;
    delay(100);
    Serial.println("booted");

    tft.init(240, 240);
    tft.setRotation(2);

    pinMode(BADGE2020_BACKLIGHT, OUTPUT);
    digitalWrite(BADGE2020_BACKLIGHT, HIGH);

    tft.fillScreen(0x2c45);
    tft.fillCircle(60, 120, 50, ST77XX_WHITE);
    tft.fillCircle(180, 120, 50, ST77XX_WHITE);

    while (accelerometer.init(LIS2DH12_RANGE_2GA) == -1) {
        // Equipment connection exception or I2C address error
        Serial.println("No I2C devices found");
        delay(1000);
    }

    strip.begin();
    strip.show();
    strip.setBrightness(50);
}

void loop() {
    currentMillis = millis();
    moveEye();
    //  blinkEye();
    rainbow();
}