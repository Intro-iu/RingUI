#include <Arduino.h>
#include "config.hpp"
#include "element.hpp"
#include "ui.hpp"
#include "menu.hpp"

// Global OLED object definition
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C OLED(U8G2_R0, U8X8_PIN_NONE, SYS_SCL, SYS_SDA);
RingController controller(OLED);

void setup() {
    pinMode(PIN_CONFIRM, INPUT_PULLDOWN);
    pinMode(PIN_CANCEL, INPUT_PULLDOWN);
    pinMode(PIN_IS_SCROLLING, INPUT_PULLDOWN);
    pinMode(PIN_SCROLL_TOWARD, INPUT_PULLUP);

    Serial.begin(115200);

    controller.setup();
    
    build_menus(controller);

    controller.handle(&mainMenu);
}

void loop() {
    // controller.handle() is blocking, so loop is empty.
}
