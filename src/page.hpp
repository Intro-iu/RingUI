/**
 * @file page.hpp
 * @brief Defines the abstract base class for all pages in the UI.
 */
#pragma once

/**
 * @class Page
 * @brief Abstract base class for all pages in the UI.
 * 
 * A page represents a screen that the user can interact with, other than a menu.
 * The RingController manages the lifecycle of pages, including drawing, animation, and input handling.
 * This base class provides a unified input handler (`handleInput`) that calls virtual on... methods
 * for subclasses to implement specific behaviors.
 */
class Page {
public:
    Page();
    virtual ~Page() {}

    /**
     * @brief Unified input handler for all pages.
     * 
     * This method is called repeatedly by the RingController's loop. It checks for
     * button and serial inputs and calls the corresponding virtual on... methods.
     * @return true if the page is finished and should be closed, false otherwise.
     */
    bool handleInput();

    /**
     * @brief Draws the page content on the screen.
     * 
     * @param y_offset The vertical offset for drawing, used for entry/exit animations.
     * The page content should be drawn at its normal coordinates plus this offset.
     */
    virtual void draw(int y_offset) = 0;

protected:
    /** @brief Called when an 'up' or 'scroll up' input is detected. */
    virtual void onScrollUp() {}

    /** @brief Called when a 'down' or 'scroll down' input is detected. */
    virtual void onScrollDown() {}

    /** 
     * @brief Called when a 'confirm' input is detected.
     * @return true if the page should close after confirmation.
     */
    virtual bool onConfirm() { return false; }

    /** 
     * @brief Called when a 'cancel' input is detected.
     * @return true if the page should close after cancellation.
     */
    virtual bool onCancel() { return true; }

private:
    unsigned long last_input_time;
};
