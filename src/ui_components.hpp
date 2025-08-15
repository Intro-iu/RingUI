/**
 * @file ui_components.hpp
 * @brief Contains various UI component classes like ProgressBar.
 */
#pragma once

#include "config.hpp"

extern DisplayDriver OLED;

/**
 * @class ProgressBar
 * @brief A simple UI component for drawing a horizontal progress bar.
 */
class ProgressBar {
public:
    ProgressBar(int x, int y, int width, int height);

    /**
     * @brief Draws the progress bar based on a percentage.
     * @param percentage The value between 0.0 and 100.0.
     * @param y_offset The vertical offset for drawing (used for page animations).
     */
    void draw(float percentage, int y_offset = 0);

    /**
     * @brief Draws the progress bar based on a value within a given range.
     * @param value The current value.
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @param y_offset The vertical offset for drawing (used for page animations).
     */
    void draw(float value, float min, float max, int y_offset = 0);

private:
    int x, y, width, height;
};
