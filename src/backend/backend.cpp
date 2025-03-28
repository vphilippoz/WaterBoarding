#include "backend.hpp"
#include <FastLED.h>

namespace backend {

// Global variables definition
bool pumps_active[] = {false, false, false, false};
CRGB led[NUM_LEDS];

// Private function declaration
void set_LED();

void setup() {
    /**
     * @brief Setup the backend of the Water Boarding project
    */
    // Configure GPIO pins as outputs and set them to inactive
    for (unsigned int i = 0; i < NUM_PUMPS; i++) {
        pinMode(PUMPS_PINS[i], OUTPUT);
        digitalWrite(PUMPS_PINS[i], LOW);
    }

    // Setup the onboard LED
    FastLED.addLeds<WS2812, LED_PIN, GRB>(led, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    led[0] = BLACK;
    FastLED.show();
}

bool toggle_pump(unsigned int pump_ID) {
    /**
     * @brief Toggle a pump and set the LED to the corresponding color
     * 
     * @param pump_ID: ID of the pump to toggle
     * @return New state of the pump (true == active)
    */
    bool pump_running = pumps_active[pump_ID];

    if(pump_running) {
        // Stop the pump and set LED to black
        digitalWrite(PUMPS_PINS[pump_ID], LOW);
    } else {
        // Start the pump
        digitalWrite(PUMPS_PINS[pump_ID], HIGH);
    }

    // Update pump state
    pump_running = !pump_running;
    pumps_active[pump_ID] = pump_running;
    Serial.println("Toggled PIN " + String(PUMPS_PINS[pump_ID]));

    // Update LED color
    set_LED();
    
    return pump_running;
}

void set_LED() {
    /**
     * @brief Toggle a pump and set the LED to the corresponding color
    */
    unsigned int red_level = 0;
    unsigned int green_level = 0;
    unsigned int blue_level = 0;

    // Determine the color levels from pumps states
    if(pumps_active[0]) {red_level += BRIGHTNESS/2;}
    if(pumps_active[1]) {green_level += BRIGHTNESS/2;}
    if(pumps_active[2]) {blue_level += BRIGHTNESS;}
    if(pumps_active[3]) {red_level += BRIGHTNESS/2; green_level += BRIGHTNESS/5;}

    led[0] = CRGB(red_level, green_level, blue_level);
    FastLED.show();
}

} // namespace backend
