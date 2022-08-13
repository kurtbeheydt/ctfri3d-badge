#include <Arduino.h>
#include <Adafruit_NeoPixel.h>  //neopixel lib
#include <Keypad.h>  //keypad lib

/*
const byte ROWS = 4;  // four rows
const byte COLS = 4;  // four columns

char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'},
};

byte colPins[ROWS] = {26, 13, 27, 2};  // connect the column pinouts to the rows of the keypad
byte rowPins[COLS] = {15, 14, 0, 32};  // connect the row pinouts to the columns of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
*/

#define LEDPIN 6     // leds of the 4x4 addon
#define NUMPIXELS 9

Adafruit_NeoPixel strip(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);          // strip for the 4x4 leds
uint32_t warmwhite = strip.Color(255, 191, 0);

void setup() {
    Serial.begin(115200);

    strip.begin();      // INITIALIZE NeoPixel strip 4x4 keyb
    strip.setBrightness(10);
    strip.show();
}

// This will hold down all the following buttons.
void sendMacroCommand(uint8_t key) {
    Serial.println(key);
}

void loop() {
    strip.fill(warmwhite, 0, 9);
    strip.show();

    for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
        strip.setPixelColor(i, warmwhite);
        strip.show();
        delay(200);
        strip.setPixelColor(i, strip.Color(0,0,0));
    }

/*
    char key = keypad.getKey();

    // Only do something with the keypress if we
    // are connected to something via bluetooth
*/
}