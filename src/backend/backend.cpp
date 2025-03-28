#include "backend.hpp"
#include <FastLED.h>

namespace backend {

// Global variables definition
bool pumps_active[] = {false, false, false, false};
CRGB led[NUM_LEDS];

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

        // Turn off LED corresponding color
        set_LED(pump_ID, false);
    } else {
        // Start the pump
        digitalWrite(PUMPS_PINS[pump_ID], HIGH);

        // Set LED to corresponding color
        set_LED(pump_ID, true);
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
     * @param LED_ON: If the LED should be active or not
    */
    if(LED_ON) {
        switch (pump_ID)
        {
        case 0:
            led[0] = RED;
            break;
        case 1:
            led[0] = GREEN;
            break;
        case 2:
            led[0] = BLUE;
            break;
        case 3:
            led[0] = YELLOW;
            break;
        default:
            led[0] = WHITE;
            break;
        }
    } else {
        led[0] = BLACK;
    }
    FastLED.show();
}

} // namespace backend
