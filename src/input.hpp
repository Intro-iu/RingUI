#pragma once

#include <Arduino.h>

enum class RotaryDirection {
    NOROTATION = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1
};

class RotaryEncoder {
public:
    RotaryEncoder(int pinA, int pinB, int pinButton, int pulsesPerDetent = 4);
    void begin();
    RotaryDirection getDirection();
    bool isPressed();

private:
    static void IRAM_ATTR readEncoder();
    static RotaryEncoder* instance;

    int _pinA;
    int _pinB;
    int _pinButton;
    int _pulsesPerDetent;

    volatile int _lastEncoded = 0;
    volatile long _encoderValue = 0;
    volatile int _direction = 0;
    
    unsigned long _lastButtonPress = 0;
    int _lastButtonState = HIGH;
};

extern RotaryEncoder g_encoder;

bool is_button_pressed(int pin);
