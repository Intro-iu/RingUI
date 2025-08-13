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
    // Check for cancel/confirm buttons first, as they are single-press events.
    if (is_button_pressed(PIN_CANCEL)) {
        return onCancel();
    }
    if (is_button_pressed(PIN_CONFIRM)) {
        return onConfirm();
    }

    // Handle continuous scrolling/value changes from buttons
    if (millis() - last_input_time > 100) {
        last_input_time = millis();
        if (digitalRead(PIN_IS_SCROLLING) == LOW) {
            if (digitalRead(PIN_SCROLL_TOWARD) == HIGH) { // Down
                onScrollDown();
            } else { // Up
                onScrollUp();
            }
        }
    }

    // Handle serial input
    if (g_config.use_serial_control) {
        char serial_cmd = get_serial_input();
        if (serial_cmd != 0) {
            switch (serial_cmd) {
                case 'w':
                    onScrollUp();
                    break;
                case 's':
                    onScrollDown();
                    break;
                case 'e':
                    return onConfirm();
                case 'q':
                    return onCancel();
            }
        }
    }

    return false; // By default, the page does not exit. Subclass methods determine exit.
}
