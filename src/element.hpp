#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
#include "page.hpp" // Required for std::function<Page*()>

class Menu; // Forward declaration

/**
 * @class MenuItem
 * @brief Represents a single item in a Menu.
 */
class MenuItem {
public:
    /**
     * @brief Defines the behavior of a MenuItem upon selection.
     */
    enum class ItemType {
        OPTION,    //< Triggers an action, usually showing a Page.
        DIRECTORY, //< Opens a submenu.
        SWITCH     //< Toggles a boolean value on and off.
    };

    String label;
    ItemType type;
    Menu* subMenu;
    
    // Action to perform for an OPTION item, which returns a new Page to be displayed.
    std::function<Page*()> action;
    // Optional callback to execute after the created Page is closed.
    std::function<void()> on_close_callback;
    // Action to perform for a SWITCH item.
    std::function<void()> switch_action;
    // Function to get the current state of a SWITCH item for display.
    std::function<bool()> get_switch_state;

    /**
     * @brief Construct a new MenuItem that functions as an option.
     * @param label The text to display for the item.
     * @param action A function that creates and returns a new Page object to be displayed.
     * @param on_close_callback An optional function to call after the page is closed.
     */
    MenuItem(String label, std::function<Page*()> action, std::function<void()> on_close_callback = nullptr);

    /**
     * @brief Construct a new MenuItem that opens a submenu.
     * @param label The text to display for the item.
     * @param subMenu A pointer to the submenu to open.
     */
    MenuItem(String label, Menu* subMenu);

    /**
     * @brief Construct a new MenuItem that functions as a switch.
     * @param label The text to display for the item.
     * @param switch_action A function to toggle the switch's state.
     * @param get_switch_state A function to get the current state of the switch.
     */
    MenuItem(String label, std::function<void()> switch_action, std::function<bool()> get_switch_state);
};

/**
 * @class Menu
 * @brief A container for a list of MenuItem objects.
 */
class Menu {
public:
    Menu(String title);

    void addItem(const MenuItem& item);
    void setParent(Menu* parent);

    MenuItem& getItem(int index);
    Menu* getParent() const;
    String getTitle() const;
    int size() const;

    // The index of the currently selected item in the menu.
    int selected = 0;

private:
    String title;
    Menu* parent;
    std::vector<MenuItem> items;
};