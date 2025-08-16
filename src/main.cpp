/**
 * @file main.cpp
 * @brief Main entry point for the RingUI application.
 * @defgroup Main
 */
#include <Arduino.h>
#include "config.hpp"
#include "menu.hpp"
#include "ui.hpp"
#include "input.hpp"
#include "pages.hpp"
#include <functional>

/// @brief Global U8g2 display driver object.
/// @ingroup Main
DisplayDriver OLED(U8G2_R0, U8X8_PIN_NONE, SYS_SCL, SYS_SDA);
/// @brief Global UI controller, which manages all menus and pages.
/// @ingroup Main
RingController<DisplayDriver> controller(OLED);

/**
 * @defgroup Menus Menu Instances
 * @ingroup Main
 * @brief Global instances of the Menu class, forming the application's menu structure.
 * @{
 */
Menu mainMenu("Main Menu");
Menu settingsMenu("Settings");
Menu displayMenu("Display");
Menu systemMenu("System");
Menu pidMenu("PID Settings");
Menu scrollPidMenu("Scroll PID");
Menu animPidMenu("Animation PID");
/** @} */

/**
 * @brief The standard Arduino setup function. Runs once on startup.
 * @ingroup Main
 */
void setup() {
    // Initialize hardware and software services.
    pinMode(PIN_CANCEL, INPUT_PULLDOWN);
    Serial.begin(115200);
    g_encoder.begin();
    controller.setup();
    
    // Create the menu structure
    {
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

        auto callback = []() { controller.update_pid_gains(); };

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

    // Start the UI controller. This is a blocking call that runs the main UI loop.
    controller.handle(&mainMenu);
}

/**
 * @brief The standard Arduino loop function.
 * @ingroup Main
 */
void loop() {
    // The controller.handle() function in setup() contains the main application
    // loop, so this function is intentionally left empty.
}

