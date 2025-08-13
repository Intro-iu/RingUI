/**
 * @file pages.cpp
 * @brief Implements the page classes used in the application.
 */
#include "pages.hpp"
#include "config.hpp"
#include <Arduino.h>

// --- InfoPage Implementation ---

InfoPage::InfoPage(String content) : content(content) {
    entry_time = millis();
}

bool InfoPage::handleInput() {
    // Wait for a short delay to prevent accidental exit right after entering the page.
    if (millis() - entry_time < 300) {
        return false;
    }

    // Exit on any button press.
    if (digitalRead(PIN_CONFIRM) == HIGH || digitalRead(PIN_CANCEL) == HIGH) {
        delay(50); // Debounce
        while(digitalRead(PIN_CONFIRM) == HIGH || digitalRead(PIN_CANCEL) == HIGH);
        return true; // Signal that the page is finished.
    }
    return false;
}

void InfoPage::draw(int y_offset) {
    // Erase the area under the page by drawing a black box.
    OLED.setDrawColor(0);
    OLED.drawBox(0, y_offset, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Draw the text content in white.
    OLED.setDrawColor(1);
    OLED.setFont(u8g2_font_6x12_me);

    int current_line_num = 0;
    int start_pos = 0;
    int newline_pos;

    while ((newline_pos = content.indexOf('\n', start_pos)) != -1) {
        OLED.setCursor(0, DEFAULT_TEXT_HEIGHT * (current_line_num + 1) + y_offset);
        OLED.print(content.substring(start_pos, newline_pos));
        start_pos = newline_pos + 1;
        current_line_num++;
    }
    // Print the last part of the string
    OLED.setCursor(0, DEFAULT_TEXT_HEIGHT * (current_line_num + 1) + y_offset);
    OLED.print(content.substring(start_pos));
    
    OLED.setFont(DEFAULT_TEXT_FONT);
}

// --- EditFloatPage Implementation ---

EditFloatPage::EditFloatPage(const char* title, float* value, float step, float min, float max)
    : title(title), 
      value_ptr(value), 
      current_value(*value), 
      step(step), min(min), max(max),
      show_progress(min != max),
      progress_bar(0, DEFAULT_TEXT_HEIGHT * 2 + 2, SCREEN_WIDTH, DEFAULT_PROGRESS_HEIGHT) 
{
    last_input_time = 0;
}

bool EditFloatPage::handleInput() {
    if (millis() - last_input_time > 100) {
        last_input_time = millis();
        if (digitalRead(PIN_IS_SCROLLING) == LOW) {
            if (digitalRead(PIN_SCROLL_TOWARD) == HIGH) { // UP
                current_value += step;
            } else { // DOWN
                current_value -= step;
            }
            if (show_progress) {
                current_value = constrain(current_value, min, max);
            }
        }
    }

    if (digitalRead(PIN_CONFIRM) == HIGH) {
        delay(50); // Debounce
        while(digitalRead(PIN_CONFIRM) == HIGH);
        *value_ptr = current_value; // Save the new value
        return true; // Signal that the page is finished.
    }

    if (digitalRead(PIN_CANCEL) == HIGH) {
        delay(50); // Debounce
        while(digitalRead(PIN_CANCEL) == HIGH);
        return true; // Signal that the page is finished, without saving.
    }

    return false;
}

void EditFloatPage::draw(int y_offset) {
    // Erase the area under the page by drawing a black box.
    OLED.setDrawColor(0);
    OLED.drawBox(0, y_offset, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Draw the text content in white.
    OLED.setDrawColor(1);
    OLED.setFont(u8g2_font_6x12_me);
    OLED.setCursor(0, DEFAULT_TEXT_HEIGHT + y_offset);
    OLED.print(title);
    OLED.setCursor(0, DEFAULT_TEXT_HEIGHT * 2 + y_offset);
    OLED.print("Value: ");
    OLED.print(current_value, 3);

    if (show_progress) {
        // The progress bar is drawn with the current draw color (white).
        progress_bar.draw(current_value, min, max, y_offset);
    }
    
    OLED.setFont(DEFAULT_TEXT_FONT);
}

// --- RebootPage Implementation ---

RebootPage::RebootPage() {
    entry_time = millis();
}

bool RebootPage::handleInput() {
    // Allow canceling within 3 seconds.
    if (millis() - entry_time < 3000) {
        if (digitalRead(PIN_CANCEL) == HIGH) {
            delay(50); // Debounce
            while(digitalRead(PIN_CANCEL) == HIGH);
            return true; // Page is finished, reboot is canceled.
        }
    } else {
        // Timeout reached, reboot now.
        ESP.restart();
    }
    return false; // This page technically never finishes.
}

void RebootPage::draw(int y_offset) {
    // Erase the area under the page by drawing a black box.
    OLED.setDrawColor(0);
    OLED.drawBox(0, y_offset, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Draw the text content in white.
    OLED.setDrawColor(1);
    OLED.setFont(u8g2_font_6x12_me);
    OLED.setCursor(0, DEFAULT_TEXT_HEIGHT + y_offset);
    OLED.print("Rebooting...");
    OLED.setCursor(0, DEFAULT_TEXT_HEIGHT * 2 + y_offset);
    OLED.print("Press CANCEL");

    OLED.setFont(DEFAULT_TEXT_FONT);
}
