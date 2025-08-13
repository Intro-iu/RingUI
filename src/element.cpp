#include "element.hpp"

MenuItem::MenuItem(String label, std::function<Page*()> action)
    : label(label), type(ItemType::OPTION), subMenu(nullptr), action(action) {}

MenuItem::MenuItem(String label, Menu* subMenu)
    : label(label), type(ItemType::DIRECTORY), subMenu(subMenu), action(nullptr) {}

MenuItem::MenuItem(String label, std::function<void()> switch_action, std::function<bool()> get_switch_state)
    : label(label), type(ItemType::SWITCH), subMenu(nullptr), action(nullptr), switch_action(switch_action), get_switch_state(get_switch_state) {}

Menu::Menu(String title) : title(title), parent(nullptr), selected(0) {}

void Menu::addItem(const MenuItem& item) {
    items.push_back(item);
    if (item.type == MenuItem::ItemType::DIRECTORY && item.subMenu != nullptr) {
        item.subMenu->setParent(this);
    }
}

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