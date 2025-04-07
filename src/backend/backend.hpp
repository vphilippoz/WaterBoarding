#ifndef BACKEND_H
#define BACKEND_H

#include <Arduino.h>

namespace backend {
    // Constants
    constexpr unsigned int NUM_PUMPS = 4;       // Number of pumps
    constexpr unsigned int NUM_LEDS = 1;        // Number of LEDs
    constexpr unsigned int BRIGHTNESS = 100;    // LEDs brightness, 0-255

    // Pinout
    constexpr unsigned int PUMPS_PINS[] =   {5, 4, 3, 2};// Pins to control the water pumps
    constexpr unsigned int LED_PIN =         8;          // Pin for onboard RGB LED

    // Colors
    #define BLACK   CHSV(0, 0, 0)

    // Global variables declaration
    // extern bool pumps_active[];

    // Function prototypes
    void setup(bool verbose);
    bool toggle_pump(unsigned int pump_ID);
    bool set_pump(unsigned int pump_ID);
    bool reset_pump(unsigned int pump_ID);

    
} // namespace knobs

#endif // BACKEND_H