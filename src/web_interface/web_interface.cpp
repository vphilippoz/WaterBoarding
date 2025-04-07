#include "web_interface.hpp"

namespace web_interface {

// Global variables definition
WebServer server(PORT);
bool (*global_pump_toggler)(unsigned int) = nullptr;
unsigned int NUM_PUMPS = 0;
bool VERBOSE = false;
const char index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>Controle d'arrosage</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            margin: 0;
            font-family: Arial, sans-serif;
        }
        .button-container {
            display: flex;
            flex-direction: column;
            gap: 20px;
        }
        button {
            padding: 15px 30px;
            font-size: 18px;
            cursor: pointer;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 5px;
            width: 200px;
        }
        button:hover {
            opacity: 0.8;
        }
        .red {
            background-color: rgb(218, 0, 0);
        }
    </style>
</head>
<body>
    <h1 class="title">Controle d'arrosage</h1>
    <div class="button-container">
        <button id="button0" onclick="sendRequest(0)">Demarrer la pompe 1</button>
        <button id="button1" onclick="sendRequest(1)">Demarrer la pompe 2</button>
        <button id="button2" onclick="sendRequest(2)">Demarrer la pompe 3</button>
        <button id="button3" onclick="sendRequest(3)">Demarrer la pompe 4</button>
    </div>
    <script>
        function sendRequest(buttonNumber) {
            fetch('/button' + buttonNumber)
                .then(response => response.text())
                .then(data => {
                    console.log('Response:', data);
                    updateButtonAppearance(buttonNumber, data);
                })
                .catch(error => console.error('Error:', error));
        }

        function updateButtonAppearance(buttonNumber, data) {
            const button = document.getElementById('button'+buttonNumber);
            buttonNumberTxt = buttonNumber + 1
            if (data === 'inactive') {
                button.textContent = 'Demarrer la pompe ' + buttonNumberTxt;
                button.classList.remove('red');
            } else if (data === 'active') {
                button.textContent = 'Arreter la pompe ' + buttonNumberTxt;
                button.classList.add('red');
            }
        }
    </script>
</body>
</html>
)rawliteral";

// Private function declaration
void handle_root(void);
void handle_toggle(unsigned int pump_ID, bool (*pump_toggler)(unsigned int));

void setup(bool (*pump_toggler)(unsigned int), unsigned int num_pumps, bool verbose) {
    /**
     * @brief Setup the web interface
    */
    // Save the pointer to pump toggler function, which ensures that the 
    // toggler function is always accessible in the handle_toggle function
    global_pump_toggler = pump_toggler;
    NUM_PUMPS = num_pumps;
    VERBOSE = verbose;

    // Connect to WiFi network
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        if(VERBOSE) {Serial.println("Connecting to WiFi...");}
    }
    if(VERBOSE) {
        Serial.println("Connected to WiFi");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }

    server.on("/", HTTP_GET, handle_root);

    for (int i = 0; i < NUM_PUMPS; i++) {
        server.on(String("/button") + String(i), HTTP_GET,
                  [i]() { handle_toggle(i, global_pump_toggler); });
    }

    // Start the web server
    server.begin();
    if(VERBOSE) {Serial.println("Web server started");}
}

void handle_root() {
    /**
     * @brief Handler for the root page
    */
    server.send(200, "text/html", index_html);
}

void handle_toggle(unsigned int pump_ID, bool (*pump_toggler)(unsigned int)) {
    /**
     * @brief Handler for the one of the toggling page
     * 
     * @param pump_ID: ID of the pump toggled
    */
    // Toggle the pump
    bool pump_state = pump_toggler(pump_ID);

    if(VERBOSE) {Serial.println("Toggled pump " + String(pump_ID+1));}
    
    // Send response
    // server.send(200, "text/plain", "Pump " + String(pump_ID+1) + " pressed");
    server.send(200, "text/plain", pump_state ? "active" : "inactive");

    // return pump_state ? "active" : "inactive";
}

} // namespace web_interface
