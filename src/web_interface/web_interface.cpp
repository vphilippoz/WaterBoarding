#include <ArduinoJson.h>
#include "web_interface.hpp"

namespace web_interface {

// Global variables definition
WebServer server(PORT);
bool (*global_pump_toggler)(unsigned int) = nullptr;
bool (*global_reset_pump)(unsigned int) = nullptr;
bool (*global_deliver_ml)(unsigned int, unsigned int) = nullptr;
unsigned int NUM_PUMPS = 0;
bool VERBOSE = true;
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
    <h2 class="title">Quantite d'eau definie</h2>
    <div class="button-container">
        <label for="selectPump"> Pompe a utiliser :</label>
        <select id="selectPump" name="Pompe">
            <option value=0 selected>Pompe 1</option>
            <option value=1>Pompe 2</option>
            <option value=2>Pompe 3</option>
            <option value=3>Pompe 4</option>
        </select>
        <label for="sliderQuantity"> Quantite d'eau a verser : <span id="sliderQuantityValue">250</span> mL</label>
        <input id="sliderQuantity" type="range" min="1" max="20" value="10" class="slider">
    </div>
    <br>
    <div class="button-container">
        <button id="buttonStart" onclick="startDelivery()">Arroser</button>
        <button id="buttonCancel" hidden="hidden" onclick="cancelDelivery()">Annuler</button>
    </div>
    <br>
    <h2 class="title">Mode manuel</h2>
    <div class="button-container">
        <button id="button0" onclick="togglePump(0)">Demarrer la pompe 1</button>
        <button id="button1" onclick="togglePump(1)">Demarrer la pompe 2</button>
        <button id="button2" onclick="togglePump(2)">Demarrer la pompe 3</button>
        <button id="button3" onclick="togglePump(3)">Demarrer la pompe 4</button>
    </div>
    <script>
        // Get elements from document
        const sliderQuantity = document.getElementById("sliderQuantity");
        const selectPump = document.getElementById("selectPump");
        const buttonStart = document.getElementById('buttonStart');
        const buttonCancel = document.getElementById('buttonCancel');

        // Update the slider value display when the slider is moved
        sliderQuantity.addEventListener("input", function() {
            sliderQuantityValue.textContent = sliderQuantity.value*25;
        });

        function togglePump(pumpID) {           
            // Send request to server for stop the selected pump
            let url = '/togglePump?pump_id=' + pumpID;
            fetch(url, {
                method: 'POST'
            })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Network response was not ok');
                }
                return response.text();
            })
            .then(data => {
                updateButtonAppearance(pumpID, data);
            })
            .catch(error => {
                console.error('There was a problem with the fetch operation:', error);
            });
        }

        function startDelivery() {
            // Get user parameters
            let ml = sliderQuantity.value * 25;
            let pumpID = selectPump.value;

            // Deactivate button "delivery"
            buttonStart.setAttribute('disabled', 'disabled');

            // Show button "cancel"
            buttonCancel.classList.add('red');
            buttonCancel.removeAttribute('hidden');

            // Send request to server for given quantity on selected pump
            let url = '/deliver_ml?quantity=' + ml + '&pump_id=' + pumpID;
            fetch(url, {
                method: 'POST'
            })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Network response was not ok');
                }
                return response.text();
            })
            .then(data => {
                // Handle the response data from the server
                console.log('Server response:', data);

                if (data === 'true') {
                    // Return to original appearance regardless of success or failure
                    buttonStart.removeAttribute('disabled');
                    buttonCancel.classList.remove('red');
                    buttonCancel.setAttribute('hidden', 'hidden');
                }
            })
            .catch(error => {
                console.error('There was a problem with the fetch operation:', error);
            });
        }

        function cancelDelivery() {
            // Get pump ID
            let pumpID = selectPump.value;
            
            // Send request to server for stop the selected pump
            let url = '/deliver_ml_cancel?pump_id=' + pumpID;
            fetch(url, {
                method: 'POST'
            })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Network response was not ok');
                }
                return response.text();
            })
            .then(data => {
                // Handle the response data from the server
                console.log('Server response:', data);

                if (data === 'true') {
                    // Return to original appearance regardless of success or failure
                    buttonStart.removeAttribute('disabled');
                    buttonCancel.classList.remove('red');
                    buttonCancel.setAttribute('hidden', 'hidden');
                }
            })
            .catch(error => {
                console.error('There was a problem with the fetch operation:', error);
            });
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
void handle_root();
void handle_toggle();
void handle_cancel();
void handle_deliver();

void setup(bool (*pump_toggler)(unsigned int), bool (*deliver_ml)(unsigned int, unsigned int), bool (*reset_pump)(unsigned int), unsigned int num_pumps, bool verbose) {
    /**
     * @brief Setup the web interface
    */
    // Save the pointer to backend functions, which ensures that the 
    // backend functions are always accessible in the handler functions
    global_pump_toggler = pump_toggler;
    global_deliver_ml = deliver_ml;
    global_reset_pump = reset_pump;
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
    server.on("/togglePump", HTTP_POST, handle_toggle);
    server.on("/deliver_ml", HTTP_POST, handle_deliver);
    server.on("/deliver_ml_cancel", HTTP_POST, handle_cancel);

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

void handle_toggle() {
    /**
     * @brief Handler for one of the toggling button
     */
    if(server.method() == HTTP_POST) {
        // Check if wanted parameters are present
        if (!server.hasArg("pump_id")) {
            Serial.println("Missing parameters");
            server.send(400, "text/plain", "Missing parameters");
            return;
        }

        // Retrieve parameters
        unsigned int pumpID = server.arg("pump_id").toInt();

        // Execute command
        bool pumpState = global_pump_toggler(pumpID);

        // Send Response
        if (pumpState) {
            server.send(200, "text/plain", "active"); // HTTP 200: OK
        } else {
            server.send(200, "text/plain", "inactive"); // HTTP 200: OK
        }
    } else {
        server.send(405, "text/plain", "Method Not Allowed"); // HTTP 405: Method Not Allowed (only POST allowed)
    }
}


void handle_deliver() {
    /**
     * @brief Handler for the deliver button
     */
    if(server.method() == HTTP_POST) {
        // Check if wanted parameters are present
         if (!server.hasArg("quantity") || !server.hasArg("pump_id")) {
            Serial.println("Missing parameters");
            server.send(400, "text/plain", "Missing parameters");
            return;
        }

        // Retrieve parameters
        unsigned int quantity = server.arg("quantity").toInt();
        unsigned int pumpID = server.arg("pump_id").toInt();

        // Execute command
        bool deliverySuccess = global_deliver_ml(pumpID, quantity);

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

void handle_cancel() {
    /**
     * @brief Handler for one of the cancel button
    */
    if(server.method() == HTTP_POST) {
        // Check if wanted parameters are present
         if (!server.hasArg("pump_id")) {
            Serial.println("Missing parameters");
            server.send(400, "text/plain", "Missing parameters");
            return;
        }

        // Retrieve parameters
        unsigned int pumpID = server.arg("pump_id").toInt();

        // Execute command
        bool pumpActive = global_reset_pump(pumpID);

        // Send Response
        if (!pumpActive) {
            server.send(200, "text/plain", "true"); // HTTP 200: OK
        } else {
            server.send(500, "text/plain", "false"); // HTTP 500: Internal Server Error
        }
    } else {
        server.send(405, "text/plain", "Method Not Allowed"); // HTTP 405: Method Not Allowed (only POST allowed)
    }
}

} // namespace web_interface
