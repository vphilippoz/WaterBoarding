/*
  WaterBoarding.ino - Example code for the Water Boarding project.
  Created by Vincent Philippoz.

  This code is made for the ESP32-C6 platform and is intended to be used with the Water Boarding PCB.
*/

#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Patate";
const char* password = "0123456789";

// Create a web server object that listens for HTTP requests on port 80
WebServer server(80);

// Define GPIO pins
const int gpioPins[] = {4, 5, 6, 7}; // Pump control PINs

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Configure GPIO pins as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(gpioPins[i], OUTPUT);
    digitalWrite(gpioPins[i], LOW); // Initialize all GPIOs to LOW
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Define web server routes
  server.on("/", handleRoot);
  for (int i = 0; i < 4; i++) {
    server.on(String("/toggle/") + i, [i]() { handleToggle(i); });
  }

  // Start the web server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}

void handleRoot() {
  String html = "<html><body>";
  for (int i = 0; i < 4; i++) {
    html += "<a href=\"/toggle/" + String(i) + "\">Toggle GPIO " + String(gpioPins[i]) + "</a><br>";
  }
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleToggle(int pinIndex) {
  int pin = gpioPins[pinIndex];
  int state = digitalRead(pin);
  digitalWrite(pin, !state); // Toggle GPIO state

  Serial.println("Toggled GPIO " + String(pin));
  
  server.sendHeader("Location", "/");
  server.send(303);
}