#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include <Arduino.h>


class Actuators {
    
    public:
        Actuators(const byte *pins, const float* parameters = NULL);
        void updatePWM(const int& pwm_left, const int& pwm_right);
        void stop();
    
    private:
        byte pin_IN1, pin_IN2, pin_EN12, pin_IN3, pin_IN4, pin_EN34;
};

#endif  // ACTUATORS_H_