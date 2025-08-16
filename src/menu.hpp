/**
 * @file menu.hpp
 * @brief Defines the Menu and MenuItem classes.
 * @defgroup MenuSystem
 * @{
 */
#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
#include "pages.hpp" // Required for std::function<Page*()>

class Menu; // Forward declaration

/**
 * @class MenuItem
 * @brief Represents a single item in a Menu.
 * @ingroup MenuSystem
 */
class MenuItem {
public:
    /**
     * @brief Defines the behavior of a MenuItem upon selection.
     */
    enum class ItemType {
        OPTION,    ///< Triggers an action, usually showing a Page.
        DIRECTORY, ///< Opens a submenu.
        SWITCH     ///< Toggles a boolean value on and off.
    };

    String label; ///< The text to display for the item.
    ItemType type; ///< The type of the menu item, which determines its behavior.
    Menu* subMenu; ///< A pointer to the submenu, if this item is a DIRECTORY.
    
    /// Action to perform for an OPTION item, which returns a new Page to be displayed.
    std::function<Page*()> action;
    /// Optional callback to execute after the created Page is closed.
    std::function<void()> on_close_callback;
    /// Action to perform for a SWITCH item.
    std::function<void()> switch_action;
    /// Function to get the current state of a SWITCH item for display.
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
 * @brief A container for a list of MenuItem objects, representing a single screen of options.
 * @ingroup MenuSystem
 */
class Menu {
public:
    /**
     * @brief Construct a new Menu object.
     * @param title The title of the menu (not currently displayed, but used for identification).
     */
    Menu(String title);

    /**
     * @brief Adds a new item to the menu's list.
     * @param item The MenuItem object to add.
     */
    void addItem(const MenuItem& item);
    /**
     * @brief Sets the parent menu for this menu, enabling backward navigation.
     * @param parent A pointer to the parent Menu.
     */
    void setParent(Menu* parent);

    /**
     * @brief Gets a reference to a menu item by its index.
     * @param index The index of the item to retrieve.
     * @return A reference to the MenuItem.
     */
    MenuItem& getItem(int index);
    /**
     * @brief Gets a pointer to the parent menu.
     * @return A pointer to the parent Menu, or nullptr if it's a root menu.
     */
    Menu* getParent() const;
    /**
     * @brief Gets the title of the menu.
     * @return The menu's title string.
     */
    String getTitle() const;
    /**
     * @brief Gets the number of items in the menu.
     * @return The number of items.
     */
    int size() const;

    /// The index of the currently selected item in the menu.
    int selected = 0;

private:
    String title; ///< The title of the menu.
    Menu* parent; ///< A pointer to the parent menu.
    std::vector<MenuItem> items; ///< The list of items in this menu.
};
/** @} */