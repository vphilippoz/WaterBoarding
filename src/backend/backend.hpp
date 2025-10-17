#ifndef BACKEND_H
#define BACKEND_H

#include <Arduino.h>
#include <FastLED.h>
#include <time.h>

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

    // Date and time
    constexpr unsigned int TIMEZONE_OFFSET = 1;   // Timezone offset in seconds
    constexpr unsigned int DAYLIGHT_SAVINGS_OFFSET = 0; // Daylight savings offset in seconds
    constexpr char* NTP_SERVER = "pool.ntp.org";   // NTP server address

    // Watering schedule (one entry per pump)
    constexpr bool WATERING_ACTIVE[] =              {true, true, false, false}; // Whether the pump should follow the schedule
    constexpr unsigned int WATERING_VOLUME_ML[] =    {1000, 1000, 1000, 1000}; // Volume of water to dispense (in mL)
    constexpr unsigned int WATERING_HOUR[] =        {13, 18, 18, 18}; // Hour of the day to water the plants (0-23)
    constexpr unsigned int WATERING_MINUTE[] =      {36, 0, 0, 0};  // Minute of the hour to water the plants (0-59)

    // Global variables declaration
    // extern bool pumps_active[];

    // Function prototypes
    void setup(bool verbose);
    bool toggle_pump(unsigned int pump_ID);
    bool set_pump(unsigned int pump_ID);
    bool reset_pump(unsigned int pump_ID);
    void deliver_ml(unsigned int pump_ID, unsigned int volume_ml);
    void handle_schedule();

} // namespace backend

#endif // BACKEND_H