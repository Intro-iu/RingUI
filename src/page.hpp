/**
 * @file page.hpp
 * @brief Defines the abstract base class for all pages in the UI.
 */
#pragma once

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