#pragma once
#include <U8g2lib.h>

// Screen dimensions
static constexpr int SCREEN_WIDTH = 128;
static constexpr int SCREEN_HEIGHT = 32;

// Default components properties
static constexpr auto DEFAULT_TEXT_FONT = u8g2_font_6x12_me;
static constexpr int DEFAULT_TEXT_WIDTH = 6;
static constexpr int DEFAULT_TEXT_HEIGHT = 12;
static constexpr int DEFAULT_TEXT_MARGIN = 2;
static constexpr int DEFAULT_PROGRESS_HEIGHT = 4;

// Initial cursor position
static constexpr int INIT_CURSOR_X = 0;
static constexpr int INIT_CURSOR_Y = 0;

// Device interfaces
static constexpr int SYS_SCL = 32;
static constexpr int SYS_SDA = 33;
static constexpr int OLED_ADDR = 0x3C;
static constexpr int CTRL_ADDR = 0x4C;

static constexpr int ANIMATION_DELAY = 10;

// Hardware pins
static constexpr int PIN_CANCEL = 27;
static constexpr int PIN_ENCODER_BUTTON = 14;
static constexpr int PIN_ENCODER_A = 13;
static constexpr int PIN_ENCODER_B = 12;

// Configuration struct
struct AppConfig {
    float scroll_pid_kp;
    float scroll_pid_ki;
    float scroll_pid_kd;
    float anim_pid_kp;
    float anim_pid_ki;
    float anim_pid_kd;
    bool use_serial_control;
};

extern AppConfig g_config;