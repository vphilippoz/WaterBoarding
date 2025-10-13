#include "backend.hpp"
#include <FastLED.h>

namespace backend {

// Global variables definition
bool VERBOSE = false;
bool pumps_active[] = {false, false, false, false};
CRGB led[NUM_LEDS];

// Private function declaration
void set_LED();

void setup(bool verbose) {
    /**
     * @brief Setup the backend of the Water Boarding project
    */
    // Save verbose mode
    VERBOSE = verbose;

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

    // Initialize time synchronisation
    configTime(0, 0, "pool.ntp.org");
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
    if(VERBOSE) {Serial.println("High on PIN " + String(PUMPS_PINS[pump_ID]));}

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
    if(VERBOSE) {Serial.println("Low on PIN " + String(PUMPS_PINS[pump_ID]));}

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

void deliver_ml(unsigned int pump_ID, unsigned int volume_ml) {
    /**
     * @brief Delivers a specific volume of water through the specified pump
     * 
     * @param pump_ID: ID of the pump to use
     * @param volume_ml: Volume of water to deliver (in mL)
    */
    // To be implemented
}

void handle_schedule() {
    /**
     * @brief Follow a predefined watering schedule
    */
    // Get current time
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
        if(VERBOSE) {Serial.println("Failed to obtain time");}
        return;
    }
    static unsigned int last_handled_minute = 61; // Initialize to an invalid minute to ensure the schedule is handled at startup

    // Only handle the schedule once per minute
    if(timeinfo.tm_min == last_handled_minute) {return;}
    last_handled_minute = timeinfo.tm_min;

    // Check each pump's schedule
    for (unsigned int pump_ID = 0; pump_ID < NUM_PUMPS;) {
        if(WATERING_ACTIVE[pump_ID]) {
            // Check if it's time to water
            if(timeinfo.tm_hour == WATERING_HOUR[pump_ID] && timeinfo.tm_min == WATERING_MINUTE[pump_ID]) {
                if(VERBOSE) {Serial.println("Scheduled watering for pump " + String(pump_ID+1));}
                
                // Deliver the specified volume of water
                deliver_ml(pump_ID, WATERING_VOLUME_ML[pump_ID]);

                if(VERBOSE) {Serial.println("Delivered " + String(WATERING_VOLUME_ML[pump_ID]) + " mL of water with pump " + String(pump_ID+1));}
            }
        }
    }
}

} // namespace backend
