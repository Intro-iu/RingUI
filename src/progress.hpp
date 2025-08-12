#pragma once

#include <U8g2lib.h>

// Forward-declare the specific OLED driver class
extern U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C OLED;

class ProgressBar {
public:
    ProgressBar(int x, int y, int width, int height);

    /**
     * @brief Draws the progress bar using a percentage.
     * @param percentage The value between 0.0 and 100.0.
     * @param y_offset The vertical offset for drawing (used for animations).
     */
    void draw(float percentage, int y_offset = 0);

    /**
     * @brief Draws the progress bar using a value and a range.
     * @param value The current value.
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @param y_offset The vertical offset for drawing (used for animations).
     */
    void draw(float value, float min, float max, int y_offset = 0);

private:
    int x, y, width, height;
};
