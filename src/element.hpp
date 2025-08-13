#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
#include "page.hpp" // Include the new Page base class

class Menu; // Forward declaration

/**
 * @class MenuItem
 * @brief Represents an item in a menu, which can be an option or a link to a submenu.
 */
class MenuItem {
public:
    enum class ItemType {
        OPTION,    // An item that triggers an action (shows a page).
        DIRECTORY, // An item that opens a submenu.
        SWITCH     // An item that toggles a boolean value.
    };

    String label;
    ItemType type;
    Menu* subMenu;
    // The action to perform when an OPTION item is selected.
    // It should return a pointer to a new Page object.
    std::function<Page*()> action;
    // An optional callback to execute when the page is closed.
    std::function<void()> on_close_callback;
    // The action to perform when a SWITCH item is selected.
    std::function<void()> switch_action;
    // A function to get the current state of the switch.
    std::function<bool()> get_switch_state;

    /**
     * @brief Construct a new MenuItem for an option.
     * @param label The text to display for the item.
     * @param action A function that creates and returns a new Page object.
     * @param on_close_callback An optional function to call when the page is closed.
     */
    MenuItem(String label, std::function<Page*()> action, std::function<void()> on_close_callback = nullptr);

    /**
     * @brief Construct a new MenuItem for a submenu link.
     * @param label The text to display for the item.
     * @param subMenu A pointer to the submenu to open.
     */
    MenuItem(String label, Menu* subMenu);

    /**
     * @brief Construct a new MenuItem for a switch.
     * @param label The text to display for the item.
     * @param switch_action A function to toggle the switch's state.
     * @param get_switch_state A function to get the current state of the switch.
     */
    MenuItem(String label, std::function<void()> switch_action, std::function<bool()> get_switch_state);
};

class Menu {
public:
    Menu(String title);

    void addItem(const MenuItem& item);
    void setParent(Menu* parent);

    MenuItem& getItem(int index);
    Menu* getParent() const;
    String getTitle() const;
    int size() const;

    int selected = 0;

private:
    String title;
    Menu* parent;
    std::vector<MenuItem> items;
};
