/*
Minimal version to test the backend library.

This code is a minimal version to test the backend library. It toggles the defined GPIOs and the corresponding LED color.
*/

#include "backend.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;

void setup() {
    // Open serial communications on the native USB port
    Serial.begin(SERIAL_BAUD_RATE);while(!Serial);

    // Setup the library
    backend::setup();
    Serial.println("Setup complete");
}

void loop() {
    for(unsigned int i = 0; i < backend::NUM_PUMPS; i++) {
        backend::toggle_pump(i);
        delay(500);
    }
}