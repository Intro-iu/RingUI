#include "element.hpp"
#include "ui.hpp"
#include "pages.hpp"
#include "config.hpp"
#include <functional>

// Extern declarations for all menu objects defined in menu.cpp.
// This allows them to be accessed from any file that includes this header.
extern Menu mainMenu;
extern Menu settingsMenu;
extern Menu displayMenu;
extern Menu systemMenu;
extern Menu pidMenu;
extern Menu scrollPidMenu;
extern Menu animPidMenu;

/**
 * @brief Builds and links all menus and menu items for the application.
 * 
 * This function is templated on the Driver type to remain independent of the
 * specific display driver being used, as it needs a reference to the RingController.
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

    // This callback is passed to the EditFloatPage items for PID settings.
    // It's called when the page is closed to update the PID controllers with new values.
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
