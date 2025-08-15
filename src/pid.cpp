/**
 * @file pid.cpp
 * @brief Implements the PIDController class.
 */
#include "pid.hpp"
#include <Arduino.h>

PIDController::PIDController(float kp, float ki, float kd, float integral_limit)
    : kp(kp), ki(ki), kd(kd), integral(0.0f), last_error(0.0f), integral_limit(integral_limit) {}

/**
 * @brief Sets new gain values for the controller.
 */
void PIDController::set_gains(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

/**
 * @brief Calculates the PID output for a given target and current value.
 */
float PIDController::update(float target, float current) {
    float error = target - current;
    
    integral += error;
    integral = constrain(integral, -integral_limit, integral_limit);
    
    float derivative = error - last_error;
    last_error = error;
    
    return kp * error + ki * integral + kd * derivative;
}

/**
 * @brief Resets the controller's internal state.
 */
void PIDController::reset() {
    integral = 0.0f;
    last_error = 0.0f;
}