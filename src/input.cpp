#include "input.hpp"
#include "config.hpp"

RotaryEncoder* RotaryEncoder::instance = nullptr;

RotaryEncoder g_encoder(PIN_ENCODER_A, PIN_ENCODER_B, PIN_ENCODER_BUTTON);

RotaryEncoder::RotaryEncoder(int pinA, int pinB, int pinButton) 
    : _pinA(pinA), _pinB(pinB), _pinButton(pinButton) {
    instance = this;
}

void RotaryEncoder::begin() {
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    pinMode(_pinButton, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_pinA), readEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_pinB), readEncoder, CHANGE);
}

void IRAM_ATTR RotaryEncoder::readEncoder() {
    int msb = digitalRead(instance->_pinA);
    int lsb = digitalRead(instance->_pinB);

    int encoded = (msb << 1) | lsb;
    int sum = (instance->_lastEncoded << 2) | encoded;

    if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        long val = instance->_encoderValue; val++; instance->_encoderValue = val;
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        long val = instance->_encoderValue; val--; instance->_encoderValue = val;
    }

    instance->_lastEncoded = encoded;
}

RotaryDirection RotaryEncoder::getDirection() {
    long value;
    noInterrupts();
    value = _encoderValue;
    _encoderValue = 0;
    interrupts();

    if (value > 0) return RotaryDirection::COUNTERCLOCKWISE;
    if (value < 0) return RotaryDirection::CLOCKWISE;
    return RotaryDirection::NOROTATION;
}

bool RotaryEncoder::isPressed() {
    if (digitalRead(_pinButton) == LOW && millis() - _lastButtonPress > INPUT_DELAY) {
        _lastButtonPress = millis();
        return true;
    }
    return false;
}

bool is_button_pressed(int pin) {
    if (digitalRead(pin) == HIGH) {
        delay(INPUT_DELAY); // Use configured debounce delay
        if (digitalRead(pin) == HIGH) {
            while(digitalRead(pin) == HIGH); // Wait for release
            return true;
        }
    }
    return false;
}