/**
 * @file pages.cpp
 * @brief Implements the page classes used in the application.
 */
#include "pages.hpp"
#include "config.hpp"
#include "input.hpp"
#include <Arduino.h>

// --- InfoPage Implementation ---

InfoPage::InfoPage(String content)
    : content(content), 
      total_lines(0), 
      last_input_time(0),
      target_scroll_offset(0),
      current_scroll_y(0.0),
      velocity_y(0.0),
      scroll_pid(g_config.scroll_pid_kp, g_config.scroll_pid_ki, g_config.scroll_pid_kd)
{
    entry_time = millis();
    // Calculate total_lines
    total_lines = 1;
    for (unsigned int i = 0; i < content.length(); i++) {
        if (content.charAt(i) == '\n') {
            total_lines++;
        }
    }
}

bool InfoPage::handleInput() {
    // Exit on CANCEL button press.
    if (is_button_pressed(PIN_CANCEL)) {
        return true; // Signal that the page is finished.
    }

    // Handle scrolling
    if (millis() - last_input_time > 100) { // A bit faster for smoother scrolling
        last_input_time = millis();
        if (digitalRead(PIN_IS_SCROLLING) == LOW) {
            if (digitalRead(PIN_SCROLL_TOWARD) == HIGH) { // Down
                target_scroll_offset++;
            } else { // Up
                target_scroll_offset--;
            }
            
            int visible_lines = SCREEN_HEIGHT / DEFAULT_TEXT_HEIGHT;
            int max_scroll = max(0, total_lines - visible_lines);
            target_scroll_offset = constrain(target_scroll_offset, 0, max_scroll);
        }
    }

    return false; // Page is not finished, continue displaying.
}

void InfoPage::draw(int y_offset) {
    // --- PID Animation ---
    double target_y = target_scroll_offset * DEFAULT_TEXT_HEIGHT;
    if (abs(target_y - current_scroll_y) > 0.1 || abs(velocity_y) > 0.1) {
        velocity_y = scroll_pid.update(target_y, current_scroll_y);
        current_scroll_y += velocity_y;
    } else {
        current_scroll_y = target_y;
    }

    // --- Drawing ---
    OLED.setDrawColor(0);
    OLED.drawBox(0, y_offset, SCREEN_WIDTH, SCREEN_HEIGHT);
    OLED.setDrawColor(1);
    OLED.setFont(u8g2_font_6x12_me);

    int start_pos = 0;
    int newline_pos;
    int line_num = 0;

    while (start_pos < (int)content.length()) {
        newline_pos = content.indexOf('\n', start_pos);
        String line;
        if (newline_pos == -1) {
            line = content.substring(start_pos);
            start_pos = content.length();
        } else {
            line = content.substring(start_pos, newline_pos);
            start_pos = newline_pos + 1;
        }

        // Calculate y position for the line, considering the animated scroll
        int line_y_pos = DEFAULT_TEXT_HEIGHT * (line_num + 1) - round(current_scroll_y);
        
        // Culling: Only draw lines that are visible on screen
        if (line_y_pos > -DEFAULT_TEXT_HEIGHT && line_y_pos < SCREEN_HEIGHT + DEFAULT_TEXT_HEIGHT) {
            OLED.setCursor(0, line_y_pos + y_offset);
            OLED.print(line);
        }
        
        line_num++;
    }
    
    // --- Scrollbar ---
    int visible_lines = SCREEN_HEIGHT / DEFAULT_TEXT_HEIGHT;
    if (total_lines > visible_lines) {
        OLED.drawVLine(SCREEN_WIDTH - 2, y_offset, SCREEN_HEIGHT);

        int slider_height = 5;
        int max_scroll_pixels = (total_lines - visible_lines) * DEFAULT_TEXT_HEIGHT;
        float scroll_percentage = max_scroll_pixels > 0 ? current_scroll_y / max_scroll_pixels : 0;
        
        int travel_distance = SCREEN_HEIGHT - slider_height;
        int slider_y = scroll_percentage * travel_distance;

        OLED.drawBox(SCREEN_WIDTH - 3, y_offset + slider_y, 2, slider_height);
    }
    
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

    if (is_button_pressed(PIN_CONFIRM)) {
        *value_ptr = current_value; // Save the new value
        return true; // Signal that the page is finished.
    }

    if (is_button_pressed(PIN_CANCEL)) {
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
        if (is_button_pressed(PIN_CANCEL)) {
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
