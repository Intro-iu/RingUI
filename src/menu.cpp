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

// Action wrappers to link menu items to page functions
void action_edit_scroll_p() { page_edit_float("Scroll Kp", &g_config.scroll_pid_kp, 0.01f, 0.0f, 1.0f); }
void action_edit_scroll_i() { page_edit_float("Scroll Ki", &g_config.scroll_pid_ki, 0.01f, 0.0f, 1.0f); }
void action_edit_scroll_d() { page_edit_float("Scroll Kd", &g_config.scroll_pid_kd, 0.01f, 0.0f, 1.0f); }
void action_edit_anim_p() { page_edit_float("Anim Kp", &g_config.anim_pid_kp, 0.01f, 0.0f, 1.0f); }
void action_edit_anim_i() { page_edit_float("Anim Ki", &g_config.anim_pid_ki, 0.001f, 0.0f, 1.0f); }
void action_edit_anim_d() { page_edit_float("Anim Kd", &g_config.anim_pid_kd, 0.01f, 0.0f, 1.0f); }

void build_menus() {
    mainMenu.addItem(MenuItem("Settings", &settingsMenu));
    mainMenu.addItem(MenuItem("About", page_show_info));
    mainMenu.addItem(MenuItem("Item 3"));
    mainMenu.addItem(MenuItem("Item 4"));

    settingsMenu.addItem(MenuItem("Display", &displayMenu));
    settingsMenu.addItem(MenuItem("PID", &pidMenu));
    settingsMenu.addItem(MenuItem("System", &systemMenu));

    displayMenu.addItem(MenuItem("Contrast"));
    displayMenu.addItem(MenuItem("Timeout"));

    pidMenu.addItem(MenuItem("Scroll", &scrollPidMenu));
    pidMenu.addItem(MenuItem("Animation", &animPidMenu));

    scrollPidMenu.addItem(MenuItem("Kp", action_edit_scroll_p));
    scrollPidMenu.addItem(MenuItem("Ki", action_edit_scroll_i));
    scrollPidMenu.addItem(MenuItem("Kd", action_edit_scroll_d));

    animPidMenu.addItem(MenuItem("Kp", action_edit_anim_p));
    animPidMenu.addItem(MenuItem("Ki", action_edit_anim_i));
    animPidMenu.addItem(MenuItem("Kd", action_edit_anim_d));

    systemMenu.addItem(MenuItem("Reboot", page_reboot));
    systemMenu.addItem(MenuItem("Reset"));
}