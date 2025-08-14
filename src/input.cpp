#include "input.hpp"
#include "config.hpp"

RotaryEncoder* RotaryEncoder::instance = nullptr;

// Set pulses per detent to 4, a common value for encoders.
RotaryEncoder g_encoder(PIN_ENCODER_A, PIN_ENCODER_B, PIN_ENCODER_BUTTON, 4);

RotaryEncoder::RotaryEncoder(int pinA, int pinB, int pinButton, int pulsesPerDetent) 
    : _pinA(pinA), _pinB(pinB), _pinButton(pinButton), _pulsesPerDetent(pulsesPerDetent) {
    instance = this;
}

void RotaryEncoder::begin() {
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    pinMode(_pinButton, INPUT_PULLUP);

    // Read the initial state of the encoder pins to prevent the first turn from being missed.
    int msb = digitalRead(_pinA);
    int lsb = digitalRead(_pinB);
    _lastEncoded = (msb << 1) | lsb;

    attachInterrupt(digitalPinToInterrupt(_pinA), readEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_pinB), readEncoder, CHANGE);
}

void IRAM_ATTR RotaryEncoder::readEncoder() {
    int msb = digitalRead(instance->_pinA);
    int lsb = digitalRead(instance->_pinB);

    int encoded = (msb << 1) | lsb;
    int sum = (instance->_lastEncoded << 2) | encoded;

    int increment = 0;
    if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        increment = 1; // Counter-Clockwise
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        increment = -1; // Clockwise
    }

    if (increment != 0) {
        // If the direction of rotation has changed, reset the counter.
        // This prevents leftover pulses from a previous turn in the opposite
        // direction from cancelling out the new turn.
        if (instance->_direction != 0 && instance->_direction != increment) {
            instance->_encoderValue = 0;
        }
        instance->_direction = increment;

        long val = instance->_encoderValue;
        val += increment;
        instance->_encoderValue = val;
    }

    instance->_lastEncoded = encoded;
}

RotaryDirection RotaryEncoder::getDirection() {
    long value;
    noInterrupts();
    value = _encoderValue;
    interrupts();

    // This logic accounts for encoders with multiple pulses per detent (e.g., 4).
    // It returns a direction only once a full detent has been turned.
    if (value >= _pulsesPerDetent) {
        noInterrupts();
        _encoderValue -= _pulsesPerDetent;
        interrupts();
        return RotaryDirection::COUNTERCLOCKWISE;
    }
    if (value <= -_pulsesPerDetent) {
        noInterrupts();
        _encoderValue += _pulsesPerDetent;
        interrupts();
        return RotaryDirection::CLOCKWISE;
    }
    
    return RotaryDirection::NOROTATION;
}

bool RotaryEncoder::isPressed() {
    bool triggered = false;
    int current_state = digitalRead(_pinButton);
    // Trigger on the falling edge (from HIGH to LOW)
    if (current_state == LOW && _lastButtonState == HIGH) {
        // Debounce check
        if (millis() - _lastButtonPress > 50) {
            triggered = true;
        }
        _lastButtonPress = millis();
    }
    _lastButtonState = current_state;
    return triggered;
}

bool is_button_pressed(int pin) {
    static unsigned long last_press_time = 0;
    static int last_button_state = LOW; // PULLDOWN, so LOW is released

    bool triggered = false;
    int current_state = digitalRead(pin);
    // Trigger on the rising edge (from LOW to HIGH)
    if (current_state == HIGH && last_button_state == LOW) {
        // Debounce check
        if (millis() - last_press_time > 50) {
            triggered = true;
        }
        last_press_time = millis();
    }
    last_button_state = current_state;
    return triggered;
}