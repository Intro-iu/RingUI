#pragma once

#include <U8g2lib.h>

// Forward-declare the specific OLED driver class
extern U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C OLED;

class ProgressBar {
public:
    ProgressBar(int x, int y, int width, int height);

    void draw(float percentage); // Percentage mode (0.0 to 100.0)
    void draw(float value, float min, float max); // Value mode

private:
    int x, y, width, height;
};
