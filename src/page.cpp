/**
 * @file page.cpp
 * @brief Implements the base Page class logic.
 */
#include "page.hpp"
#include "input.hpp"
#include "config.hpp"

// The Page constructor is defaulted in the header and last_input_time was removed.

bool Page::handleInput() {
    // The cancel button has the highest priority.
    if (is_button_pressed(PIN_CANCEL)) {
        return onCancel();
    }

    // Handle encoder rotation for scrolling.
    RotaryDirection dir;
    while ((dir = g_encoder.getDirection()) != RotaryDirection::NOROTATION) {
        if (dir == RotaryDirection::CLOCKWISE) {
            onScrollDown();
        } else if (dir == RotaryDirection::COUNTERCLOCKWISE) {
            onScrollUp();
        }
    }

    // Handle encoder button press for confirmation.
    if (g_encoder.isPressed()) {
        return onConfirm();
    }

    // By default, the page does not exit and continues to be displayed.
    return false;
}