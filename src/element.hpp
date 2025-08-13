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
        DIRECTORY  // An item that opens a submenu.
    };

    String label;
    ItemType type;
    Menu* subMenu;
    // The action to perform when an OPTION item is selected.
    // It should return a pointer to a new Page object.
    std::function<Page*()> action;

    /**
     * @brief Construct a new MenuItem for an option.
     * @param label The text to display for the item.
     * @param action A function that creates and returns a new Page object.
     */
    MenuItem(String label, std::function<Page*()> action);

    /**
     * @brief Construct a new MenuItem for a submenu link.
     * @param label The text to display for the item.
     * @param subMenu A pointer to the submenu to open.
     */
    MenuItem(String label, Menu* subMenu);
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
