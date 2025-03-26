#include "web_interface.hpp"

namespace web_interface {

// Global variables definition
WebServer server(PORT);

// Private function declaration
void handle_root(void);
// void handle_toggle(unsigned int pump_ID, std::function<bool(unsigned int)> pump_toggler);
void handle_toggle(unsigned int pump_ID);

void setup(std::function<bool(unsigned int)> pump_toggler) {
    /**
     * @brief Setup the web interface
    */
    // Connect to WiFi network
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Define web server routes
    server.on("/", handle_root);
    for (int i = 0; i < 4; i++) {
        // server.on(String("/toggle/") + String(i), [i, &pump_toggler]() { handle_toggle(i, pump_toggler); });
        server.on(String("/toggle/") + String(i), [i]() { handle_toggle(i); });
    }

    // Start the web server
    server.begin();
    Serial.println("Web server started");
}

void handle_root() {
  String html = "<html><body>";
  for (int i = 0; i < 4; i++) {
    html += "<a href=\"/toggle/" + String(i) + "\">Toggle pump " + String(i+1) + "</a><br>";
  }
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// void handle_toggle(unsigned int pump_ID, std::function<bool(unsigned int)> pump_toggler) {
void handle_toggle(unsigned int pump_ID) {
  // Toggle the pump
  // bool pump_state = pump_toggler(pump_ID);

  Serial.println("Toggled pump " + String(pump_ID));
  
  // Send response
  server.sendHeader("Location", "/");
  server.send(303);
}

} // namespace web_interface
