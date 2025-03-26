/*
#include "M5Atom.h"
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
 
/*
 The code is simple. It initializes the M5Atom, clears the LED, and then enters the loop. The loop turns the LED green, waits for 500ms, turns off the LED, and waits for another 500ms. 
 The code is uploaded to the M5Atom using the Arduino IDE. The M5Atom should now blink green every second. 
 Conclusion 
 The M5Atom is a small and powerful development board that can be used for various projects. It has a built-in LED display, accelerometer, and gyroscope. The M5Atom can be programmed using the Arduino IDE, which makes it easy to get started with. 
 In this tutorial, we learned how to set up the M5Atom and how to program it using the Arduino IDE. We also learned how to use the built-in LED display to display colors. 
 If you have any questions or comments, please let me know in the comments below. 
 Thanks for reading! 
 References 
 M5Atom Product Page M5Atom GitHub Repository M5Atom Arduino Library M5Atom Arduino Library Examples
 Related Posts 
 M5Stack: Getting Started with the M5Stack Core2 ESP32 IoT Development Kit ESP32: Getting Started with the ESP32 Development Board ESP32: Getting Started with the M5Stack Core2 ESP32 IoT Development
 lib_deps = 
    fastled/FastLED@3.9.9
    m5stack/M5Atom@^0.1.3
 */