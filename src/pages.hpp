/**
 * @file pages.hpp
 * @brief Declares the page classes used in the application.
 */
#pragma once

#include "page.hpp"
#include "progress.hpp"
#include <U8g2lib.h>

// Forward-declare the OLED driver to avoid including the full header.
extern U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C OLED;

/**
 * @class InfoPage
 * @brief A simple page that displays two lines of static text and waits for a key press to exit.
 */
class InfoPage : public Page {
public:
    InfoPage(String content);
    bool handleInput() override;
    void draw(int y_offset) override;

private:
    String content;
    unsigned long entry_time;
    int total_lines;
    unsigned long last_input_time;

    // Animation variables
    int target_scroll_offset; // Target line
    double current_scroll_y;  // Current scroll position in pixels
    double velocity_y;
    double integral_y;
    double last_error_y;
};

/**
 * @class EditFloatPage
 * @brief A page for editing a float value, with an optional progress bar.
 */
class EditFloatPage : public Page {
public:
    EditFloatPage(const char* title, float* value, float step, float min = 0.0f, float max = 0.0f);
    bool handleInput() override;
    void draw(int y_offset) override;

private:
    const char* title;
    float* value_ptr;      // Pointer to the original value to modify.
    float current_value;   // The value being edited.
    float step, min, max;
    bool show_progress;    // True if a min and max range is provided.
    ProgressBar progress_bar;
    unsigned long last_input_time;
};

/**
 * @class RebootPage
 * @brief A page that displays a "Rebooting..." message and then restarts the device.
 */
class RebootPage : public Page {
public:
    RebootPage();
    bool handleInput() override;
    void draw(int y_offset) override;

private:
    unsigned long entry_time; // Time of page entry to control reboot delay.
};