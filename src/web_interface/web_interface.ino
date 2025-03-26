/*
Minimal version to test the backend library.

This code is a minimal version to test the backend library. It toggles the defined GPIOs and the corresponding LED color.
*/

#include "web_interface.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;

bool dummy_toggler(unsigned int n) {return true;}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Setup the web interface
  web_interface::setup(&dummy_toggler);
  Serial.println("Web server started");
}

void loop() {
  // Handle client requests
  web_interface::server.handleClient();
}