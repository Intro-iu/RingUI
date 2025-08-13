/**
 * @file menu.cpp
 * @brief Defines and builds the menu structure for the application.
 */
#include "menu.hpp"
#include "pages.hpp"
#include "config.hpp"
#include <functional>

// Menu Definitions
Menu mainMenu("Main Menu");
Menu settingsMenu("Settings");
Menu displayMenu("Display");
Menu systemMenu("System");
Menu pidMenu("PID Settings");
Menu scrollPidMenu("Scroll PID");
Menu animPidMenu("Animation PID");

/**
 * @brief Builds the entire menu structure of the application.
 * 
 * This function creates all menu items and links them together to form
 * a hierarchical menu system.
 */
void build_menus() {
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

    scrollPidMenu.addItem(MenuItem("Kp", []() { return new EditFloatPage("Scroll Kp", &g_config.scroll_pid_kp, 0.01f, 0.0f, 1.0f); }));
    scrollPidMenu.addItem(MenuItem("Ki", []() { return new EditFloatPage("Scroll Ki", &g_config.scroll_pid_ki, 0.01f, 0.0f, 1.0f); }));
    scrollPidMenu.addItem(MenuItem("Kd", []() { return new EditFloatPage("Scroll Kd", &g_config.scroll_pid_kd, 0.01f, 0.0f, 1.0f); }));

    animPidMenu.addItem(MenuItem("Kp", []() { return new EditFloatPage("Anim Kp", &g_config.anim_pid_kp, 0.01f, 0.0f, 1.0f); }));
    animPidMenu.addItem(MenuItem("Ki", []() { return new EditFloatPage("Anim Ki", &g_config.anim_pid_ki, 0.001f, 0.0f, 1.0f); }));
    animPidMenu.addItem(MenuItem("Kd", []() { return new EditFloatPage("Anim Kd", &g_config.anim_pid_kd, 0.01f, 0.0f, 1.0f); }));

    systemMenu.addItem(MenuItem("Reboot", []() { return new RebootPage(); }));
    systemMenu.addItem(MenuItem("Serial Control", []() { g_config.use_serial_control = !g_config.use_serial_control; }, []() { return g_config.use_serial_control; }));
    systemMenu.addItem(MenuItem("Reset", [](){ return nullptr; }));
}