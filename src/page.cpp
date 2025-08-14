/**
 * @file page.cpp
 * @brief Implements the base Page class logic.
 */
#include "page.hpp"
#include "input.hpp"
#include "config.hpp"
#include <Arduino.h> // For millis()

Page::Page() : last_input_time(0) {}

bool Page::handleInput() {
    // Check for cancel button first
    if (is_button_pressed(PIN_CANCEL)) {
        return onCancel();
    }

    // Handle encoder rotation
    RotaryDirection dir;
    while ((dir = g_encoder.getDirection()) != RotaryDirection::NOROTATION) {
        if (dir == RotaryDirection::CLOCKWISE) {
            onScrollDown();
        } else if (dir == RotaryDirection::COUNTERCLOCKWISE) {
            onScrollUp();
        }
    }

    // Handle encoder button press for confirm
    if (g_encoder.isPressed()) {
        return onConfirm();
    }

    return false; // By default, the page does not exit.
}
