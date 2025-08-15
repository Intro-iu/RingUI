/**
 * @file pages.hpp
 * @brief Declares the specific Page classes used in the application.
 */
#pragma once

#include "page.hpp"
#include "progress.hpp"
#include "pid.hpp"
#include <U8g2lib.h>

// Forward-declare the OLED driver to avoid including the full header in every file.
extern U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C OLED;

/**
 * @class InfoPage
 * @brief A page that displays multi-line, scrollable text content.
 */
class InfoPage : public Page {
public:
    InfoPage(String content);
    void draw(int y_offset) override;

protected:
    void onScrollUp() override;
    void onScrollDown() override;

private:
    void constrainScroll();
    String content;
    unsigned long entry_time;
    int total_lines;

    // Scrolling animation variables
    int target_scroll_offset; // Target line index for scrolling
    double current_scroll_y;  // Current scroll position in pixels for smooth animation
    double velocity_y;
    PIDController scroll_pid;
};

/**
 * @class EditFloatPage
 * @brief A page for editing a floating-point value with an optional progress bar.
 */
class EditFloatPage : public Page {
public:
    EditFloatPage(const char* title, float* value, float step, float min = 0.0f, float max = 0.0f);
    void draw(int y_offset) override;

protected:
    void onScrollUp() override;
    void onScrollDown() override;
    bool onConfirm() override;

private:
    const char* title;
    float* value_ptr;      ///< Pointer to the original value to be modified.
    float current_value;   ///< The temporary value being edited on the page.
    float step, min, max;
    bool show_progress;    ///< True if a min and max range is provided, enabling the progress bar.
    ProgressBar progress_bar;
};

/**
 * @class RebootPage
 * @brief A page that displays a "Rebooting..." message and then restarts the device after a delay.
 */
class RebootPage : public Page {
public:
    RebootPage();
    void draw(int y_offset) override;

protected:
    bool onCancel() override;

private:
    /// Time of page entry, used to control the reboot delay.
    unsigned long entry_time;
};