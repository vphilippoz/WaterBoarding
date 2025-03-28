#ifndef BACKEND_H
#define BACKEND_H

#include <Arduino.h>

namespace backend {
    // Constants
    constexpr unsigned int NUM_PUMPS = 4;       // Number of pumps
    constexpr unsigned int NUM_LEDS = 1;        // Number of LEDs
    constexpr unsigned int BRIGHTNESS = 100;    // LEDs brightness, 0-255

    // Pinout
    constexpr unsigned int PUMPS_PINS[] =   {4, 5, 6, 7};// Pins to control the water pumps
    constexpr unsigned int LED_PIN =       8;          // Pin for onboard RGB LED

    // Colors
    #define RED   CHSV(HSVHue::HUE_RED, 255, BRIGHTNESS)
    #define GREEN CHSV(HSVHue::HUE_GREEN, 255, BRIGHTNESS)
    #define BLUE  CHSV(HSVHue::HUE_BLUE, 255, BRIGHTNESS)
    #define YELLOW CHSV(HSVHue::HUE_YELLOW, 255, BRIGHTNESS)
    #define WHITE CHSV(0, 0, BRIGHTNESS)
    #define BLACK CHSV(0, 0, 0)

    // Global variables declaration
    extern bool pumps_active[];

    // Function prototypes
    void setup(void);
    bool toggle_pump(unsigned int pump_ID);
    
} // namespace knobs

#endif // BACKEND_H