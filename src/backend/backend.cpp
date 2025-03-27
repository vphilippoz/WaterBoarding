#include "backend.hpp"

namespace backend {

// Global variables definition
bool pumps_active[] = {false, false, false, false};

// Private function declaration
void set_LED(unsigned int pump_ID, bool LED_ON=true);

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

        // Set LED to corresponding color
        set_LED(pump_ID);
    } else {
        // Start the pump
        digitalWrite(PUMPS_PINS[pump_ID], HIGH);

        // Set LED to corresponding color
        set_LED(pump_ID, false);
    }

    // Update pump state
    pump_running = !pump_running;
    pumps_active[pump_ID] = pump_running;
    Serial.println("Toggled PIN " + String(PUMPS_PINS[pump_ID]));
    
    return pump_running;
}

void set_LED(unsigned int pump_ID, bool LED_ON) {
    /**
     * @brief Toggle a pump and set the LED to the corresponding color
     * 
     * @param pump_ID: ID of the pump toggle
    */
    // TODO: Implement the function
}

} // namespace backend
