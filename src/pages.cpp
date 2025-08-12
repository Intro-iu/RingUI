#include "pages.hpp"
#include "config.hpp"
#include <Arduino.h>

void page_reboot() {
    OLED.clearBuffer();
    OLED.setCursor(0, DEFAULT_TEXT_HEIGHT);
    OLED.print("Rebooting...");
    OLED.sendBuffer();
    delay(1000);
    ESP.restart();
}

void page_show_info() {
    OLED.setDrawColor(1);
    OLED.setFont(u8g2_font_6x12_me);
    OLED.clearBuffer();
    OLED.setCursor(0, DEFAULT_TEXT_HEIGHT);
    OLED.print("ESP32 Menu Fw");
    OLED.setCursor(0, DEFAULT_TEXT_HEIGHT * 2);
    OLED.print("Version 1.0");
    OLED.sendBuffer();
    
    delay(200);
    while(digitalRead(PIN_CONFIRM) == LOW && digitalRead(PIN_CANCEL) == LOW);
    delay(50);
    while(digitalRead(PIN_CONFIRM) == HIGH || digitalRead(PIN_CANCEL) == HIGH);
    
    OLED.setFont(DEFAULT_TEXT_FONT);
}

void page_edit_float(const char* title, float* value, float step) {
    float currentValue = *value;
    unsigned long last_input = 0;

    OLED.setDrawColor(1);
    OLED.setFont(u8g2_font_6x12_me);

    while(true) {
        if (millis() - last_input > 100) {
            last_input = millis();
            if (digitalRead(PIN_IS_SCROLLING) == LOW) {
                if (digitalRead(PIN_SCROLL_TOWARD) == HIGH) { // Down
                    currentValue -= step;
                } else { // Up
                    currentValue += step;
                }
            }
        }

        if (digitalRead(PIN_CONFIRM) == HIGH) {
            delay(50);
            while(digitalRead(PIN_CONFIRM) == HIGH);
            *value = currentValue;
            break;
        }

        if (digitalRead(PIN_CANCEL) == HIGH) {
            delay(50);
            while(digitalRead(PIN_CANCEL) == HIGH);
            break;
        }

        OLED.clearBuffer();
        OLED.setCursor(0, DEFAULT_TEXT_HEIGHT);
        OLED.print(title);
        OLED.setCursor(0, DEFAULT_TEXT_HEIGHT * 2);
        OLED.print("Value: ");
        OLED.print(currentValue, 3);
        OLED.sendBuffer();
        delay(10);
    }

    OLED.setFont(DEFAULT_TEXT_FONT);
}