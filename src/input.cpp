#include "input.hpp"
#include "config.hpp"
#include <Arduino.h>

/**
 * @brief Checks if a button is pressed, with debouncing.
 * 
 * This function checks the state of a button connected to a specific pin.
 * It includes a software debounce delay to prevent multiple triggers from a single press.
 * It waits for the button to be released before returning.
 * 
 * @param pin The GPIO pin number where the button is connected.
 * @return true if the button was pressed and released, false otherwise.
 */
bool is_button_pressed(int pin) {
    if (digitalRead(pin) == HIGH) {
        delay(INPUT_DELAY); // Use configured debounce delay
        if (digitalRead(pin) == HIGH) {
            while(digitalRead(pin) == HIGH); // Wait for release
            return true;
        }
    }
    return false;
}

char get_serial_input() {
    if (Serial.available() > 0) {
        return Serial.read();
    }
    return 0;
}
