/**
 * @file ui_components.cpp
 * @brief Implements UI component classes.
 */
#include "ui_components.hpp"
#include "config.hpp"
#include <Arduino.h>

ProgressBar::ProgressBar(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

void ProgressBar::draw(float percentage, int y_offset) {
    percentage = constrain(percentage, 0.0f, 100.0f);
    float bar_width = (percentage / 100.0f) * width;
    
    OLED.drawFrame(x, y + y_offset, width, height);
    OLED.drawBox(x, y + y_offset, bar_width, height);
}

void ProgressBar::draw(float value, float min, float max, int y_offset) {
    value = constrain(value, min, max);
    float percentage = ((value - min) / (max - min)) * 100.0f;
    draw(percentage, y_offset);
}
