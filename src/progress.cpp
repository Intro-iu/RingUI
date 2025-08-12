#include "progress.hpp"
#include "config.hpp"
#include <Arduino.h> // For constrain

ProgressBar::ProgressBar(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

void ProgressBar::draw(float percentage) {
    // Ensure value is within 0-100 range
    percentage = constrain(percentage, 0.0f, 100.0f);
    float bar_width = (percentage / 100.0f) * width;
    OLED.drawFrame(x, y, width, height);
    OLED.drawBox(x, y, bar_width, height);
}

void ProgressBar::draw(float value, float min, float max) {
    // Ensure value is within min-max range
    value = constrain(value, min, max);
    float percentage = ((value - min) / (max - min)) * 100.0f;
    draw(percentage);
}
