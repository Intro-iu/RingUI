#include <Arduino.h>
#include "config.hpp"
#include "element.hpp"
#include "ui.hpp"
#include "menu.hpp"
#include "input.hpp"

// Global OLED object definition
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C OLED(U8G2_R0, U8X8_PIN_NONE, SYS_SCL, SYS_SDA);
RingController controller(OLED);

void setup() {
    pinMode(PIN_CANCEL, INPUT_PULLDOWN);

    Serial.begin(115200);

    g_encoder.begin();

    controller.setup();
    
    build_menus(controller);

    controller.handle(&mainMenu);
}

void loop() {
    // controller.handle() is blocking, so loop is empty.
}
