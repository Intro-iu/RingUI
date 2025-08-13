#pragma once

class PIDController {
public:
    PIDController(float kp, float ki, float kd, float integral_limit = 20.0f);

    void set_gains(float kp, float ki, float kd);
    float update(float target, float current);
    void reset();

private:
    float kp, ki, kd;
    float integral;
    float last_error;
    float integral_limit;
};
