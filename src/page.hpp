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
 */
class Page {
public:
    virtual ~Page() {}

    /**
     * @brief Handles user input for the page.
     * 
     * This method is called repeatedly by the RingController's loop.
     * @return true if the page is finished and should be closed, false otherwise.
     */
    virtual bool handleInput() = 0;

    /**
     * @brief Draws the page content on the screen.
     * 
     * @param y_offset The vertical offset for drawing, used for entry/exit animations.
     * The page content should be drawn at its normal coordinates plus this offset.
     */
    virtual void draw(int y_offset) = 0;
};
