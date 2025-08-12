#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>

class Menu; // Forward declaration

class MenuItem {
public:
    enum class ItemType {
        OPTION,
        DIRECTORY
    };

    String label;
    ItemType type;
    Menu* subMenu;
    std::function<void()> action;

    MenuItem(String label, std::function<void()> action = nullptr);
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
