/*
Minimal version to test the backend library.

This code is a minimal version to test the backend library. It toggles the defined GPIOs and the corresponding LED color.
*/

#include <WiFi.h>
#include "backend.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;
constexpr bool VERBOSE = true;
constexpr char* SSID = "Patate";            // WiFi network SSID
constexpr char* PASSWORD = "0123456789";    // WiFi network password

void setup() {
    if(VERBOSE) {
        // Open serial communications on the native USB port
        Serial.begin(SERIAL_BAUD_RATE);while(!Serial);
    }

    // Connect to WiFi network
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        if(VERBOSE) {Serial.println("Connecting to WiFi...");}
    }
    Serial.println("Connected to WiFi");

    // Setup the library
    backend::setup(VERBOSE);
    if(VERBOSE) {Serial.println("Setup complete");}
}

void loop() {
    // // Toggle individually each pump
    // for(unsigned int i = 0; i < backend::NUM_PUMPS; i++) {
    //     backend::toggle_pump(i);
    //     delay(300);
    //     backend::toggle_pump(i);
    //     delay(100);
    // }

    // delay(1000);

    // // Toggle all pumps
    // for(unsigned int i = 0; i < backend::NUM_PUMPS*2; i++) {
    //     backend::toggle_pump(i%backend::NUM_PUMPS);
    //     delay(200);
    // }

    // delay(1000);

    // Handle watering schedule
    // backend::handle_schedule();

    static struct tm timeinfo;
    getLocalTime(&timeinfo);
    Serial.println("Current time: " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min));
    delay(1e4);
}