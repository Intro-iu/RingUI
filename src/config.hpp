/**
 * @file config.hpp
 * @brief Contains global constants and the application configuration structure.
 * @defgroup Config
 * @{
 */
#pragma once
#include <U8g2lib.h>

//==============================================================================
// Display Properties
//==============================================================================
/**
 * @defgroup DisplayConfig Display Properties
 * @ingroup Config
 * @{
 */
/// 
/// The type definition for the display driver.
using DisplayDriver = U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C;
///
/// The width of the OLED screen in pixels.
static constexpr int SCREEN_WIDTH = 128;
///
/// The height of the OLED screen in pixels.
static constexpr int SCREEN_HEIGHT = 32;
/** @} */

//==============================================================================
// UI Layout & Styling
//==============================================================================
/**
 * @defgroup UIConfig UI Layout & Styling
 * @ingroup Config
 * @{
 */
///
/// The default font used for text rendering throughout the UI.
static constexpr auto DEFAULT_TEXT_FONT = u8g2_font_6x12_me;
///
/// The height of a single line of text using the default font.
static constexpr int DEFAULT_TEXT_HEIGHT = 12;
///
/// The margin around text within UI elements like menu items.
static constexpr int DEFAULT_TEXT_MARGIN = 2;
///
/// The height of the progress bar used in pages like EditFloatPage.
static constexpr int DEFAULT_PROGRESS_HEIGHT = 4;
///
/// The initial horizontal position for the text cursor.
static constexpr int INIT_CURSOR_X = 0;
///
/// The initial vertical position for the text cursor.
static constexpr int INIT_CURSOR_Y = 0;
/** @} */

//==============================================================================
// I2C Devices
//==============================================================================
/**
 * @defgroup I2CConfig I2C Devices
 * @ingroup Config
 * @{
 */
/// The GPIO pin connected to the I2C serial clock (SCL) line.
static constexpr int SYS_SCL = 32;
/// The GPIO pin connected to the I2C serial data (SDA) line.
static constexpr int SYS_SDA = 33;
/// The I2C address of the OLED display.
static constexpr int OLED_ADDR = 0x3C;
/// The I2C address of an auxiliary controller (not currently used).
static constexpr int CTRL_ADDR = 0x4C; // Not currently used
/** @} */

//==============================================================================
// Animation
//==============================================================================
/**
 * @defgroup AnimConfig Animation
 * @ingroup Config
 * @{
 */
/// The delay in milliseconds between animation frames.
static constexpr int ANIMATION_DELAY = 10; // ms
/** @} */

//==============================================================================
// Hardware Pins
//==============================================================================
/**
 * @defgroup PinConfig Hardware Pins
 * @ingroup Config
 * @{
 */
/// The GPIO pin for the cancel/back button.
static constexpr int PIN_CANCEL = 27;
/// The GPIO pin for the rotary encoder's push button.
static constexpr int PIN_ENCODER_BUTTON = 14;
/// The GPIO pin for the rotary encoder's A output.
static constexpr int PIN_ENCODER_A = 13;
/// The GPIO pin for the rotary encoder's B output.
static constexpr int PIN_ENCODER_B = 12;
/** @} */

/**
 * @struct AppConfig
 * @brief Holds runtime-configurable parameters, primarily PID gains for animations.
 * @ingroup Config
 */
struct AppConfig {
    // PID gains for menu scrolling animation
    float scroll_pid_kp; ///< Proportional gain for menu scrolling.
    float scroll_pid_ki; ///< Integral gain for menu scrolling.
    float scroll_pid_kd; ///< Derivative gain for menu scrolling.
    // PID gains for screen transition animations
    float anim_pid_kp;   ///< Proportional gain for page/menu transitions.
    float anim_pid_ki;   ///< Integral gain for page/menu transitions.
    float anim_pid_kd;   ///< Derivative gain for page/menu transitions.
    // System settings
    bool use_serial_control; ///< If true, allows controlling the UI via serial commands.
};

/// Global instance of the application configuration.
extern AppConfig g_config;
/** @} */