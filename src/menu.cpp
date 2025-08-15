/**
 * @file menu.cpp
 * @brief Defines and allocates memory for the global menu objects.
 */
#include "menu.hpp"

// Global menu object definitions. These are linked together in build_menus().
Menu mainMenu("Main Menu");
Menu settingsMenu("Settings");
Menu displayMenu("Display");
Menu systemMenu("System");
Menu pidMenu("PID Settings");
Menu scrollPidMenu("Scroll PID");
Menu animPidMenu("Animation PID");
