#pragma once

#include "element.hpp"
#include "ui.hpp"
#include "pages.hpp"
#include "config.hpp"
#include <functional>

// Extern declarations for all menu objects defined in menu.cpp
extern Menu mainMenu;
extern Menu settingsMenu;
extern Menu displayMenu;
extern Menu systemMenu;
extern Menu pidMenu;
extern Menu scrollPidMenu;
extern Menu animPidMenu;

/**
 * @brief Builds the entire menu structure of the application. 
 * 
 * This is a template function to be independent of the display driver.
 * The implementation is in this header file as required by C++ templates.
 */
template <typename Driver>
void build_menus(RingController<Driver>& controller) {
    mainMenu.addItem(MenuItem("Settings", &settingsMenu));
    mainMenu.addItem(MenuItem("About", []() { return new InfoPage("RingUI  v_Master\nhttps://github.com/\nIntro-iu/RingUI\nDemo: BV1EPbezSETx"); }));
    mainMenu.addItem(MenuItem("Item 3", [](){ return nullptr; }));
    mainMenu.addItem(MenuItem("Item 4", [](){ return nullptr; }));

    settingsMenu.addItem(MenuItem("Display", &displayMenu));
    settingsMenu.addItem(MenuItem("PID", &pidMenu));
    settingsMenu.addItem(MenuItem("System", &systemMenu));

    displayMenu.addItem(MenuItem("Contrast", [](){ return nullptr; }));
    displayMenu.addItem(MenuItem("Timeout", [](){ return nullptr; }));

    pidMenu.addItem(MenuItem("Scroll", &scrollPidMenu));
    pidMenu.addItem(MenuItem("Animation", &animPidMenu));

    auto callback = [&controller]() { controller.update_pid_gains(); };

    scrollPidMenu.addItem(MenuItem("Kp", []() { return new EditFloatPage("Scroll Kp", &g_config.scroll_pid_kp, 0.01f, 0.0f, 1.0f); }, callback));
    scrollPidMenu.addItem(MenuItem("Ki", []() { return new EditFloatPage("Scroll Ki", &g_config.scroll_pid_ki, 0.01f, 0.0f, 1.0f); }, callback));
    scrollPidMenu.addItem(MenuItem("Kd", []() { return new EditFloatPage("Scroll Kd", &g_config.scroll_pid_kd, 0.01f, 0.0f, 1.0f); }, callback));

    animPidMenu.addItem(MenuItem("Kp", []() { return new EditFloatPage("Anim Kp", &g_config.anim_pid_kp, 0.01f, 0.0f, 1.0f); }, callback));
    animPidMenu.addItem(MenuItem("Ki", []() { return new EditFloatPage("Anim Ki", &g_config.anim_pid_ki, 0.001f, 0.0f, 1.0f); }, callback));
    animPidMenu.addItem(MenuItem("Kd", []() { return new EditFloatPage("Anim Kd", &g_config.anim_pid_kd, 0.01f, 0.0f, 1.0f); }, callback));

    systemMenu.addItem(MenuItem("Reboot", []() { return new RebootPage(); }));
    systemMenu.addItem(MenuItem("Serial Control", []() { g_config.use_serial_control = !g_config.use_serial_control; }, []() { return g_config.use_serial_control; }));
    systemMenu.addItem(MenuItem("Reset", [](){ return nullptr; }));
}