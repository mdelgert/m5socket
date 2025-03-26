#include "M5Atom.h"
#include "AtomSocket.h"
#include <Arduino.h>

void setup() {
    M5.begin(true, false, true); // Serial, I2C, Display (LED)
    delay(50);                  // Small delay to stabilize
    M5.dis.clear();             // Clear the LED
}

void loop() {
    M5.dis.fillpix(0x00FF00); // Set LED to green (RGB: 0xRRGGBB)
    delay(500);               // Wait 500ms
    M5.dis.clear();           // Turn off LED
    delay(500);               // Wait 500ms
    Serial.println("Blinking LED");
}