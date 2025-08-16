/**
 * @file input.hpp
 * @brief Defines the RotaryEncoder class and other input helpers.
 * @defgroup Input
 * @{
 */
#pragma once

#include <Arduino.h>

/**
 * @enum RotaryDirection
 * @brief Represents the direction of rotation for the encoder.
 * @ingroup Input
 */
enum class RotaryDirection {
    NOROTATION = 0,       ///< No complete rotation detent has been detected.
    CLOCKWISE = 1,        ///< A clockwise rotation detent has been detected.
    COUNTERCLOCKWISE = -1 ///< A counter-clockwise rotation detent has been detected.
};

/**
 * @class RotaryEncoder
 * @brief Handles input from a rotary encoder with a push button.
 * @ingroup Input
 * 
 * This class uses interrupts to read encoder rotation and provides debounced
 * methods to get the direction of turn and button press state. It is designed
 * as a singleton to be used with hardware interrupts.
 */
class RotaryEncoder {
public:
    /**
     * @brief Constructs a new RotaryEncoder object.
     * @param pinA The GPIO pin for the encoder's A output.
     * @param pinB The GPIO pin for the encoder's B output.
     * @param pinButton The GPIO pin for the encoder's push button.
     * @param pulsesPerDetent The number of pulses the encoder generates per physical detent (click).
     */
    RotaryEncoder(int pinA, int pinB, int pinButton, int pulsesPerDetent = 4);

    /**
     * @brief Initializes the GPIO pins and attaches interrupts.
     */
    void begin();

    /**
     * @brief Gets the direction of the last full detent turn.
     * @return The direction of rotation, or NOROTATION if no full turn has occurred.
     */
    RotaryDirection getDirection();

    /**
     * @brief Checks if the encoder's button has been pressed.
     * 
     * This method is debounced.
     * @return true if a press event occurred, false otherwise.
     */
    bool isPressed();

private:
    /// The interrupt service routine (ISR) for reading encoder state changes.
    static void IRAM_ATTR readEncoder();
    /// Singleton instance pointer for the ISR.
    static RotaryEncoder* instance;

    int _pinA; ///< GPIO pin for encoder output A.
    int _pinB; ///< GPIO pin for encoder output B.
    int _pinButton; ///< GPIO pin for the encoder's push button.
    int _pulsesPerDetent; ///< Number of pulses per physical detent.

    volatile int _lastEncoded = 0; ///< Stores the last combined state of pins A and B.
    volatile long _encoderValue = 0; ///< Accumulates encoder pulses.
    volatile int _direction = 0; ///< Stores the current direction of rotation (1 or -1).
    
    unsigned long _lastButtonPress = 0; ///< Timestamp of the last button press for debouncing.
    int _lastButtonState = HIGH; // Assumes INPUT_PULLUP
};

/// @brief Global instance of the rotary encoder, used throughout the application.
/// @ingroup Input
extern RotaryEncoder g_encoder;

/**
 * @brief A generic, debounced button press checker for a given pin.
 * @ingroup Input
 * @param pin The GPIO pin to check, assuming it's configured with INPUT_PULLDOWN.
 * @return true if a press event occurred, false otherwise.
 */
bool is_button_pressed(int pin);
/** @} */