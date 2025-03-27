#include "web_interface.hpp"

namespace web_interface {

// Global variables definition
WebServer server(PORT);
bool (*global_pump_toggler)(unsigned int) = nullptr;
unsigned int NUM_PUMPS = 0;

// Private function declaration
void handle_root(void);
void handle_toggle(unsigned int pump_ID, bool (*pump_toggler)(unsigned int));

void setup(bool (*pump_toggler)(unsigned int), unsigned int num_pumps) {
    /**
     * @brief Setup the web interface
    */
    // Save the pointer to pump toggler function, which ensures that the 
    // toggler function is always accessible in the handle_toggle function
    global_pump_toggler = pump_toggler;
    NUM_PUMPS = num_pumps;

    // Connect to WiFi network
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Define web server routes
    server.on("/", handle_root);
    for (int i = 0; i < NUM_PUMPS; i++) {
        server.on(String("/toggle/") + String(i), 
                  [i]() { handle_toggle(i, global_pump_toggler); });
    }

    // Start the web server
    server.begin();
    Serial.println("Web server started");
}

void handle_root() {
    /**
     * @brief Handler for the root page
    */
    String html = "<html><body>";
    for (int i = 0; i < NUM_PUMPS; i++) {
        html += "<a href=\"/toggle/" + String(i) + "\">Toggle pump " + String(i+1) + "</a><br>";
    }
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handle_toggle(unsigned int pump_ID, bool (*pump_toggler)(unsigned int)) {
    /**
     * @brief Handler for the one of the toggling page
     * 
     * @param pump_ID: ID of the pump toggled
    */
    // Toggle the pump
    bool pump_state = pump_toggler(pump_ID);

    Serial.println("Toggled pump " + String(pump_ID+1));
    
    // Send response (redirect client to home page)
    server.sendHeader("Location", "/");
    server.send(303);
}

} // namespace web_interface
