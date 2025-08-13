#include "pid.hpp"
#include <Arduino.h>

PIDController::PIDController(float kp, float ki, float kd, float integral_limit)
    : kp(kp), ki(ki), kd(kd), integral(0.0f), last_error(0.0f), integral_limit(integral_limit) {}

void PIDController::set_gains(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

float PIDController::update(float target, float current) {
    float error = target - current;
    integral += error;
    integral = constrain(integral, -integral_limit, integral_limit);
    float derivative = error - last_error;
    float output = kp * error + ki * integral + kd * derivative;
    last_error = error;
    return output;
}

void PIDController::reset() {
    integral = 0.0f;
    last_error = 0.0f;
}
