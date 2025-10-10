# WaterBoarding

This project is under developpment.

When uploaded to an ESP32, this code allows any user to toggle a water pump from a web interface. The ESP32 hosts a web page such that any device on the same network can interract with it.

## Requirements

### Arduino IDE

1. Install the esp32 board support by Espressif Systems
1. Select Tools > USB CDC On Boot > Enabled from the Arduino IDE menus (required to make Serial.print work on ESP32 boards)

## Software structure

The software is split in modules.

### Backend

This module controls the pumps using GPIOs and displays the pump state on the LED.

Public functions:
- _setup_: Required to setup the pins of the board prior to any other operation.
- _toggle_pump_: Change the state of a given pump
- _set_pump_: Activate a given pump
- _reset_pump_: Deactivate a given pump
- _deliver_ml_: Activate a given pump until the desired water quantity is delivered

Private functions:
- _set_LED_: Set the LED color according to the pumps states. Is called automatically by _set_pump_ and _reset_pump_ functions.

### Web interface

This module serves an HTML page to let a user interract with the watering system. 

N.B.: Currently, the HTML page is stored as a hardcoded string of text. For developpement, it is easier to work on _/data/index.html_ and copy paste the entire file content in the string when finished.

Public functions:
- _setup_: It defines the routes by linking the HTTP requests to backend functions. Also, it handles connection to wifi.

Private functions:
- _handle_root_: Sends the HTML page content to the client at connection.
- _handle_toggle_: When the client clicks on a "Démarrer la pompe X" button, this handler is called. It calls a backend function to toggle the corresponding pump.
- _handle_deliver_: When the client clicks on "Arroser" button, this handler is called. It extracts the pump ID and the amount to deliver from the request. Then, it calls a backend function to deliver the correct amount of liquid on the corresponding pump.
- _handle_cancel_: When the client clicks on "Annuler" button, this handler is called. It extracts the pump ID from the request. Then, it calls a backend function to stop the corresponding pump.