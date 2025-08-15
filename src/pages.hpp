/**
 * @file pages.hpp
 * @brief Defines the abstract base class for all pages and declares the specific Page classes used in the application.
 */
#pragma once

#include "config.hpp"
#include "ui_components.hpp"
#include "pid.hpp"

/**
 * @class Page
 * @brief Abstract base class for all UI pages.
 * 
 * A Page represents any screen that is not a menu. The RingController manages the 
 * lifecycle of pages, including drawing, animation, and input handling. This base class 
 * provides a unified input handler (`handleInput`) that calls virtual on... methods
 * for subclasses to implement specific behaviors for scroll, confirm, and cancel events.
 */
class Page {
public:
    Page() = default;
    virtual ~Page() = default;

    /**
     * @brief Handles all user input for the page.
     * 
     * This method is called repeatedly by the RingController's main loop. It checks for
     * rotary encoder and button inputs and calls the corresponding virtual methods.
     * @return true if the page should be closed, false otherwise.
     */
    bool handleInput();

    /**
     * @brief Draws the page's content on the display.
     * @param y_offset The vertical offset for drawing, used for entry/exit animations.
     * The page content should be drawn at its normal coordinates plus this offset.
     */
    virtual void draw(int y_offset) = 0;

protected:
    /// Called when a scroll-up input is detected.
    virtual void onScrollUp() {}

    /// Called when a scroll-down input is detected.
    virtual void onScrollDown() {}

    /** 
     * @brief Called when a confirm input is detected.
     * @return true if the page should close after confirmation. Defaults to false.
     */
    virtual bool onConfirm() { return false; }

    /** 
     * @brief Called when a cancel input is detected.
     * @return true if the page should close after cancellation. Defaults to true.
     */
    virtual bool onCancel() { return true; }
};

extern DisplayDriver OLED;

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
