#include "web_interface.hpp"

namespace web_interface {

// Global variables definition
WebServer server(PORT);
bool (*global_pump_toggler)(unsigned int) = nullptr;
bool (*global_reset_pump)(unsigned int, unsigned int) = nullptr;
bool (*global_deliver_ml)(unsigned int) = nullptr;
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
            opacity: 0.7
        }
        button:hover {
            opacity: 1;
        }
        .red {
            background-color: #E00000;
        }
        .slider {
            -webkit-appearance: none;
            width: 100%;
            height: 15px;
            border-radius: 5px;  
            background: #d3d3d3;
            outline: none;
            opacity: 0.7;
            -webkit-transition: .2s;
            transition: opacity .2s;
        }
        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 25px;
            height: 25px;
            border-radius: 50%; 
            background: #04AA6D;
            cursor: pointer;
        }
        .slider::-moz-range-thumb {
            width: 25px;
            height: 25px;
            border-radius: 50%;
            background: #04AA6D;
            cursor: pointer;
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
    <div class="button-container">
        <label> Choisir une pompe :</label>
        <select id="selectPump" name="Pompe">
            <option value=0 selected>Pompe 1</option>
            <option value=1>Pompe 2</option>
            <option value=2>Pompe 3</option>
            <option value=3>Pompe 4</option>
        </select>

        <label> Choisir une quantité d'eau :</label>
        <input id="sliderQuantity" type="range" min="1" max="20" value="10" class="slider">
        <button id="buttonDeliver" onclick="startDelivery()">Arroser</button>
        <button id="buttonDeliverCancel" hidden="hidden" onclick="cancelDelivery()">Annuler</button>
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

        function startDelivery() {
            // Get slider quantity
            const slider = document.getElementById("sliderQuantity");
            let ml = slider.value*25;

            // Get pump ID
            const select = document.getElementById("selectPump");
            let pumpID = select.value;

            // Send request to server for given quantity

            // Deactivate button "delivery"
            const buttonStart = document.getElementById('buttonDeliver');
            buttonStart.setAttribute('disabled', 'disabled')

            // Show button "cancel"
            const buttonCancel = document.getElementById('buttonDeliverCancel');
            buttonCancel.classList.add('red');
            buttonCancel.removeAttribute('hidden');

            // Wait for server response ...

            // Return to original appearance
            buttonStart.removeAttribute('disabled')
            buttonCancel.setAttribute('hidden', 'hidden');
        }

        function updateButtonAppearance(buttonNumber, data) {
            const button = document.getElementById('button'+buttonNumber);
            let buttonNumberTxt = buttonNumber + 1;
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
void handle_cancel(unsigned int pump_ID, bool (*pump_toggler)(unsigned int));
void handle_deliver();

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
                  [i]() {handle_toggle(i, global_pump_toggler); });
    }
    server.on("/deliver_ml", HTTP_POST, handle_deliver);
    server.on("/deliver_ml_cancel", HTTP_POST,
              []() {});

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
     * @brief Handler for one of the toggling button
     * 
     * @param pump_ID: ID of the pump toggled
     * @param pump_toggler: Pointer to backend function that performs the toggling 
    */
    // Toggle the pump
    bool pump_state = pump_toggler(pump_ID);

    if(VERBOSE) {Serial.println("Toggled pump " + String(pump_ID+1));}
    
    // Send response
    server.send(200, "text/plain", pump_state ? "active" : "inactive");
}

void handle_deliver(unsigned int pump_ID, unsigned int ml, void (*pump_deliverer)(unsigned int, unsigned int)) {
    /**
     * @brief Handler for the deliver button
     * 
     * @param pump_ID: ID of the pump to dispense from
     * @param ml: Quantity of water to dispense, in milliliters
     * @param pump_deliverer: Pointer to backend function that performs the delivery 
    */
    if(server.method() == HTTP_POST) {
        String body = server.arg("plain"); // Get the request body as a String

        // JSON Parsing using ArduinoJson
        StaticJsonDocument<200> doc; // Adjust size as needed (check with https://arduinojson.org/v6/assistant/)
        DeserializationError error = deserializeJson(doc, body);

        if(error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            server.send(400, "text/plain", "Invalid JSON"); // HTTP 400: Bad Request
            return;
        }

        // Extract data
        if(!doc.containsKey("quantity") || !doc["quantity"].is<int>() ||
            !doc.containsKey("pump_id") || !doc["pump_id"].is<int>()) {
            Serial.println("Missing or invalid parameters");
            server.send(400, "text/plain", "Missing or invalid parameters");
            return;
        }

        // Retrieve parameters
        unsigned int quantity = doc["quantity"];
        unsigned int pumpID = doc["pump_id"];

        // Execute command
        bool deliverySuccess = global_deliver_ml(pump_ID, quantity);

        // Send Response
        if (deliverySuccess) {
            server.send(200, "text/plain", "true"); // HTTP 200: OK
        } else {
            server.send(500, "text/plain", "false"); // HTTP 500: Internal Server Error
        }
    } else {
        server.send(405, "text/plain", "Method Not Allowed"); // HTTP 405: Method Not Allowed (only POST allowed)
    }
}

void handle_cancel(unsigned int pump_ID, bool (*pump_stopper)(unsigned int)) {
    /**
     * @brief Handler for one of the cancel button
     * 
     * @param pump_ID: ID of the pump to stop
     * @param pump_toggler: Pointer to backend function that performs the stopping 
    */
    // Dispense the water
    pump_stopper(pump_ID);

    // Send response
    server.send(200, "text/plain", 'true');
}

} // namespace web_interface
