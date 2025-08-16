/**
 * @file pages.hpp
 * @brief Defines the abstract base class for all pages and declares the specific Page classes used in the application.
 * @defgroup Pages
 * @{
 */
#pragma once

#include "config.hpp"
#include "ui_components.hpp"
#include "pid.hpp"

/**
 * @class Page
 * @brief Abstract base class for all UI pages.
 * @ingroup Pages
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
    /// @brief Called when a scroll-up input is detected.
    /// @details Subclasses should override this to handle upward scrolling or value decrementing.
    virtual void onScrollUp() {}

    /// @brief Called when a scroll-down input is detected.
    /// @details Subclasses should override this to handle downward scrolling or value incrementing.
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

/// @brief Global U8g2 display driver object, used by pages for drawing.
/// @ingroup Pages
extern DisplayDriver OLED;

/**
 * @class InfoPage
 * @brief A page that displays multi-line, scrollable text content.
 * @ingroup Pages
 */
class InfoPage : public Page {
public:
    /**
     * @brief Construct a new Info Page object
     * @param content The multi-line string content to display.
     */
    InfoPage(String content);
    void draw(int y_offset) override;

protected:
    void onScrollUp() override;
    void onScrollDown() override;

private:
    /// @brief Constrains the target scroll offset to be within the valid range.
    void constrainScroll();
    String content; ///< The text content displayed on the page.
    unsigned long entry_time; ///< The time when the page was created.
    int total_lines; ///< The total number of lines in the content string.

    // Scrolling animation variables
    int target_scroll_offset; ///< Target line index for scrolling.
    double current_scroll_y;  ///< Current scroll position in pixels for smooth animation.
    double velocity_y; ///< Current scroll velocity.
    PIDController scroll_pid; ///< PID controller for smooth scrolling animation.
};

/**
 * @class EditFloatPage
 * @brief A page for editing a floating-point value with an optional progress bar.
 * @ingroup Pages
 */
class EditFloatPage : public Page {
public:
    /**
     * @brief Construct a new Edit Float Page object
     * @param title The title displayed at the top of the page.
     * @param value A pointer to the float value to be edited.
     * @param step The increment/decrement amount for each scroll event.
     * @param min The minimum allowed value. If min and max are different, a progress bar is shown.
     * @param max The maximum allowed value. If min and max are different, a progress bar is shown.
     */
    EditFloatPage(const char* title, float* value, float step, float min = 0.0f, float max = 0.0f);
    void draw(int y_offset) override;

protected:
    void onScrollUp() override;
    void onScrollDown() override;
    bool onConfirm() override;

private:
    const char* title; ///< The title text displayed on the page.
    float* value_ptr;      ///< Pointer to the original value to be modified.
    float current_value;   ///< The temporary value being edited on the page.
    float step, min, max;  ///< Parameters for value editing: step, min, and max range.
    bool show_progress;    ///< True if a min and max range is provided, enabling the progress bar.
    ProgressBar progress_bar; ///< The progress bar component.
};

/**
 * @class RebootPage
 * @brief A page that displays a "Rebooting..." message and then restarts the device after a delay.
 * @ingroup Pages
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
/** @} */
