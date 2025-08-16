/**
 * @file config.cpp
 * @brief Contains the initialization for the global application configuration.
 */
#include "config.hpp"

/// @brief Global instance of the application configuration, with default values.
AppConfig g_config = {
    .scroll_pid_kp = 0.2f,
    .scroll_pid_ki = 0.0f,
    .scroll_pid_kd = 0.1f,
    .anim_pid_kp = 0.25f,
    .anim_pid_ki = 0.0f,
    .anim_pid_kd = 0.15f,
    .use_serial_control = true
};