/*
Minimal version to test the web_interface library.

This code is a minimal version to test the web_interface library. It toggles the defined GPIOs and the corresponding LED color.
*/

#include "web_interface.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;
constexpr unsigned int NUM_PUMPS = 4;
constexpr bool DEBUG = false;


bool dummy_toggler(unsigned int n) {return true;}

void setup() {
    if(DEBUG) {
        // Initialize serial communication
        Serial.begin(115200);
        while(!Serial) {delay(200);};
    }

    // Setup the web interface
    web_interface::setup(&dummy_toggler, NUM_PUMPS, DEBUG);
    if(DEBUG) {Serial.println("Web server started");}
}

void loop() {
    // Handle client requests
    web_interface::server.handleClient();
}