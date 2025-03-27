/*
  WaterBoarding.ino - Example code for the Water Boarding project.
  Created by Vincent Philippoz.

  This code is made for the ESP32-C6 platform and is intended to be used with the Water Boarding PCB.
*/

#include "src\web_interface\web_interface.hpp"
#include "src\backend\backend.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;

void setup() {
  // Initialize serial communication
  Serial.begin(SERIAL_BAUD_RATE);

  backend::setup();
  web_interface::setup(backend::toggle_pump);
}

void loop() {
  // Handle client requests
  web_interface::server.handleClient();
}