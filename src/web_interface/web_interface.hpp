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
    void setup(std::function<bool(unsigned int)> pump_toggler);
    
} // namespace web_interface

#endif // WEB_INTERFACE_H