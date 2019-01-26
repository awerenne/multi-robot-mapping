#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include <Arduino.h>

#include "Sensors.h"

class Sensors;  // To avoid problem of recursive definition

class Actuators {
    
 public:
    Actuators(const byte *pins, const float* parameters = NULL);
    void FollowLine(const int& error);
    void OneInch();
    void Stop();
    void Turn(const bool& clockwise, const bool& full, Sensors* sensors=NULL);
    void UpdateSpeed(const float& velocity_left, const float& velocity_right); 
    void UpdatePWM(const int& pwm_left, const int& pwm_right);
    

 private:
    byte pin_IN1, pin_IN2, pin_EN12, pin_IN3, pin_IN4, pin_EN34;
    int previous_error, accumulated_error;
    bool new_line; // Too avoid problem at init between error & previous error & reset distance for each segment
    float kp, kd, ki, pid_value, target_speed;
    float distance;

    void TurnToLine(const bool& clockwise, Sensors* sensors);
};

#endif  // ACTUATORS_H_