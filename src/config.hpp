#pragma once
#include <U8g2lib.h>

// Screen dimensions
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   32

// Default font properties
#define DEFAULT_TEXT_FONT   u8g2_font_6x12_me
#define DEFAULT_TEXT_WIDTH  6
#define DEFAULT_TEXT_HEIGHT 12
#define DEFAULT_TEXT_MARGIN 2

// Initial cursor position
#define INIT_CURSOR_X   0
#define INIT_CURSOR_Y   0

// Device interfaces
#define SYS_SCL         18
#define SYS_SDA         19
#define OLED_ADDR       0x3C
#define CTRL_ADDR       0x4C

#define INPUT_DELAY     150
#define ANIMATION_DELAY 10

// Hardware pins
#define PIN_CONFIRM       14
#define PIN_CANCEL        15
#define PIN_IS_SCROLLING  0
#define PIN_SCROLL_TOWARD 17

// Configuration struct
struct AppConfig {
    float scroll_pid_kp;
    float scroll_pid_ki;
    float scroll_pid_kd;
    float anim_pid_kp;
    float anim_pid_ki;
    float anim_pid_kd;
};

extern AppConfig g_config;