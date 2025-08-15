/**
 * @file config.hpp
 * @brief Contains global constants and the application configuration structure.
 */
#pragma once
#include <U8g2lib.h>

//==============================================================================
// Display Properties
//==============================================================================
using DisplayDriver = U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C;
static constexpr int SCREEN_WIDTH = 128;
static constexpr int SCREEN_HEIGHT = 32;

//==============================================================================
// UI Layout & Styling
//==============================================================================
static constexpr auto DEFAULT_TEXT_FONT = u8g2_font_6x12_me;
static constexpr int DEFAULT_TEXT_HEIGHT = 12;
static constexpr int DEFAULT_TEXT_MARGIN = 2;
static constexpr int DEFAULT_PROGRESS_HEIGHT = 4;
static constexpr int INIT_CURSOR_X = 0;
static constexpr int INIT_CURSOR_Y = 0;

//==============================================================================
// I2C Devices
//==============================================================================
static constexpr int SYS_SCL = 32;
static constexpr int SYS_SDA = 33;
static constexpr int OLED_ADDR = 0x3C;
static constexpr int CTRL_ADDR = 0x4C; // Not currently used

//==============================================================================
// Animation
//==============================================================================
static constexpr int ANIMATION_DELAY = 10; // ms

//==============================================================================
// Hardware Pins
//==============================================================================
static constexpr int PIN_CANCEL = 27;
static constexpr int PIN_ENCODER_BUTTON = 14;
static constexpr int PIN_ENCODER_A = 13;
static constexpr int PIN_ENCODER_B = 12;

/**
 * @struct AppConfig
 * @brief Holds runtime-configurable parameters, primarily PID gains for animations.
 */
struct AppConfig {
    // PID gains for menu scrolling animation
    float scroll_pid_kp;
    float scroll_pid_ki;
    float scroll_pid_kd;
    // PID gains for screen transition animations
    float anim_pid_kp;
    float anim_pid_ki;
    float anim_pid_kd;
    // System settings
    bool use_serial_control;
};

/// Global instance of the application configuration.
extern AppConfig g_config;