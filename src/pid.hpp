#pragma once

/**
 * @class PIDController
 * @brief A simple Proportional-Integral-Derivative (PID) controller.
 * 
 * Used for creating smooth, organic-looking animations for UI elements
 * by calculating an "effort" value to move a current value towards a target.
 */
class PIDController {
public:
    /**
     * @brief Constructs a new PIDController.
     * @param kp Proportional gain.
     * @param ki Integral gain.
     * @param kd Derivative gain.
     * @param integral_limit The maximum absolute value for the integral term to prevent windup.
     */
    PIDController(float kp, float ki, float kd, float integral_limit = 20.0f);

    /**
     * @brief Sets new gain values for the controller.
     */
    void set_gains(float kp, float ki, float kd);

    /**
     * @brief Calculates the PID output for a given target and current value.
     * @param target The desired value.
     * @param current The current value.
     * @return The calculated output (e.g., velocity) to apply to the current value.
     */
    float update(float target, float current);

    /**
     * @brief Resets the controller's internal state (integral and last error).
     */
    void reset();

private:
    float kp, ki, kd;
    float integral;
    float last_error;
    float integral_limit;
};