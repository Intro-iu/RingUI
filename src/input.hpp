#pragma once

#include <Arduino.h>

/**
 * @enum RotaryDirection
 * @brief Represents the direction of rotation for the encoder.
 */
enum class RotaryDirection {
    NOROTATION = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1
};

/**
 * @class RotaryEncoder
 * @brief Handles input from a rotary encoder with a push button.
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

    int _pinA;
    int _pinB;
    int _pinButton;
    int _pulsesPerDetent;

    volatile int _lastEncoded = 0;
    volatile long _encoderValue = 0;
    volatile int _direction = 0;
    
    unsigned long _lastButtonPress = 0;
    int _lastButtonState = HIGH; // Assumes INPUT_PULLUP
};

/// Global instance of the rotary encoder.
extern RotaryEncoder g_encoder;

/**
 * @brief A generic, debounced button press checker for a given pin.
 * @param pin The GPIO pin to check, assuming it's configured with INPUT_PULLDOWN.
 * @return true if a press event occurred, false otherwise.
 */
bool is_button_pressed(int pin);