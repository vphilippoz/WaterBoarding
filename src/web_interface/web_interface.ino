/*
Minimal version to test the web_interface library.

This code is a minimal version to test the web_interface library. It toggles the defined GPIOs and the corresponding LED color.
*/

#include "web_interface.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;
constexpr unsigned int NUM_PUMPS = 4;
constexpr bool DEBUG = false;


bool dummy_toggler(unsigned int n) {Serial.print("Toggling pump "); Serial.println(n); return true;}
bool dummy_deliver_ml(unsigned int n, unsigned int ml) {Serial.print("Delivering "); Serial.print(ml); Serial.print("ml to pump "); Serial.println(n); return true;}
bool dummy_reset_pump(unsigned int n) {Serial.print("Resetting pump "); Serial.println(n); return true;}

void setup() {
    if(DEBUG) {
        // Initialize serial communication
        Serial.begin(115200);
        while(!Serial) {delay(200);};
    }

    // Setup the web interface
    web_interface::setup(&dummy_toggler, &dummy_deliver_ml, &dummy_reset_pump, NUM_PUMPS, DEBUG);
    if(DEBUG) {Serial.println("Web server started");}
}

void loop() {
    // Handle client requests
    web_interface::server.handleClient();
}