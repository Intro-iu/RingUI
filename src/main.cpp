/**
 * @file main.cpp
 * @brief Main entry point for the RingUI application.
 */
#include <Arduino.h>
#include "config.hpp"
#include "element.hpp"
#include "ui.hpp"
#include "menu.hpp"
#include "input.hpp"

// Global U8g2 display driver object.
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C OLED(U8G2_R0, U8X8_PIN_NONE, SYS_SCL, SYS_SDA);
// Global UI controller, which manages all menus and pages.
RingController controller(OLED);

/**
 * @brief The standard Arduino setup function. Runs once on startup.
 */
void setup() {
    // Initialize hardware and software services.
    pinMode(PIN_CANCEL, INPUT_PULLDOWN);
    Serial.begin(115200);
    g_encoder.begin();
    controller.setup();
    
    // Create the menu structure.
    build_menus(controller);

    // Start the UI controller. This is a blocking call that runs the main UI loop.
    controller.handle(&mainMenu);
}

/**
 * @brief The standard Arduino loop function.
 */
void loop() {
    // The controller.handle() function in setup() contains the main application
    // loop, so this function is intentionally left empty.
}