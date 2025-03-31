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
     * @brief Toggles a pump and update the LED color correspondingly
     * 
     * @param pump_ID: ID of the pump to toggle
     * @return New state of the pump (true == active)
    */
    bool pump_running = pumps_active[pump_ID];

    if(pump_running) {
        // Stop the pump and set LED to black
        return reset_pump(pump_ID);
    } else {
        // Start the pump
        return set_pump(pump_ID);
    }
}

bool set_pump(unsigned int pump_ID) {
    /**
     * @brief Activates a pump and update the LED color correspondingly
     * 
     * @param pump_ID: ID of the pump to activate
     * @return New state of the pump (true == active)
    */
    bool pump_running = pumps_active[pump_ID];

    // Do nothing if the pump is already active
    if(pump_running) {return true;}

    // Start the pump
    digitalWrite(PUMPS_PINS[pump_ID], HIGH);

    // Update pump state
    pumps_active[pump_ID] = true;
    Serial.println("High on PIN " + String(PUMPS_PINS[pump_ID]));

    // Update LED color
    set_LED();
    
    return true;
}

bool reset_pump(unsigned int pump_ID) {
    /**
     * @brief Deactivates a pump and update the LED color correspondingly
     * 
     * @param pump_ID: ID of the pump to deactivate
     * @return New state of the pump (true == active)
    */
    bool pump_running = pumps_active[pump_ID];

    // Do nothing if the pump is already inactive
    if(!pump_running) {return false;}

    // Stop the pump
    digitalWrite(PUMPS_PINS[pump_ID], LOW);

    // Update pump state
    pumps_active[pump_ID] = false;
    Serial.println("Low on PIN " + String(PUMPS_PINS[pump_ID]));

    // Update LED color
    set_LED();
    
    return false;
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
