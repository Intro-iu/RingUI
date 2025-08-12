#pragma once

#include <U8g2lib.h>

// Forward-declare the specific OLED driver class to avoid including the full header in other headers.
extern U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C OLED;

void page_reboot();
void page_show_info();
void page_edit_float(const char* title, float* value, float step, float min = 0.0f, float max = 0.0f);
