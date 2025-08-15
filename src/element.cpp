/**
 * @file element.cpp
 * @brief Implements the Menu and MenuItem classes.
 */
#include "element.hpp"

MenuItem::MenuItem(String label, std::function<Page*()> action, std::function<void()> on_close_callback)
    : label(label), type(ItemType::OPTION), subMenu(nullptr), action(action), on_close_callback(on_close_callback) {}

MenuItem::MenuItem(String label, Menu* subMenu)
    : label(label), type(ItemType::DIRECTORY), subMenu(subMenu), action(nullptr) {}

MenuItem::MenuItem(String label, std::function<void()> switch_action, std::function<bool()> get_switch_state)
    : label(label), type(ItemType::SWITCH), subMenu(nullptr), action(nullptr), switch_action(switch_action), get_switch_state(get_switch_state) {}

Menu::Menu(String title) : title(title), parent(nullptr), selected(0) {}

/**
 * @brief Adds a new item to the menu's list.
 * @param item The MenuItem object to add.
 */
void Menu::addItem(const MenuItem& item) {
    items.push_back(item);
    if (item.type == MenuItem::ItemType::DIRECTORY && item.subMenu != nullptr) {
        item.subMenu->setParent(this);
    }
}

/**
 * @brief Sets the parent menu for this menu, enabling backward navigation.
 * @param parent A pointer to the parent Menu.
 */
void Menu::setParent(Menu* parent) {
    this->parent = parent;
}

MenuItem& Menu::getItem(int index) {
    return items[index];
}

Menu* Menu::getParent() const {
    return parent;
}

String Menu::getTitle() const {
    return title;
}

int Menu::size() const {
    return items.size();
}
