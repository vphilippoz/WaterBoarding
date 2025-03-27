#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

namespace web_interface {
    // Constants
    constexpr char* SSID = "Patate";            // WiFi network SSID
    constexpr char* PASSWORD = "0123456789";    // WiFi network password
    constexpr unsigned int PORT = 80;           // Port for the server

    // Global variables declaration
    extern WebServer server;

    // Function prototypes
    void setup(bool (*pump_toggler)(unsigned int));
    
} // namespace web_interface

#endif // WEB_INTERFACE_H