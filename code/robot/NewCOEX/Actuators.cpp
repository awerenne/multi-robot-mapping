#include "Actuators.h"


//============
Actuators::Actuators(const byte *pins, const float* parameters=NULL) { 
    pin_IN1 = pins[0]; 
    pin_IN2 = pins[1];
    pin_EN12 = pins[2];
    pinMode(pin_EN12, OUTPUT);
    pinMode(pin_IN1, OUTPUT);
    pinMode(pin_IN2, OUTPUT);
    pin_IN3 = pins[3];
    pin_IN4 = pins[4];
    pin_EN34 = pins[5];
    pinMode(pin_EN34, OUTPUT);
    pinMode(pin_IN3, OUTPUT);
    pinMode(pin_IN4, OUTPUT);
    stop();
}


//============
void Actuators::stop() {
    digitalWrite(pin_IN1, HIGH);
    digitalWrite(pin_IN2, HIGH); 
    digitalWrite(pin_IN3, HIGH);
    digitalWrite(pin_IN4, HIGH); 
}


//============
void Actuators::validPWM(int& pwm) {
    if (pwm > 255) pwm = 255;
    else if (pwm < -255) pwm = -255;
}


//============
void Actuators::updatePWM(int pwm_left, int pwm_right) {
    validPWM(pwm_right);
    if (pwm_right >= 0) {
        digitalWrite(pin_IN1, HIGH);
        digitalWrite(pin_IN2, LOW); 
    }
    else {
        digitalWrite(pin_IN1, LOW);
        digitalWrite(pin_IN2, HIGH); 
    }
    analogWrite(pin_EN12, abs(pwm_right));

    validPWM(pwm_left);
    if (pwm_left >= 0) {
        digitalWrite(pin_IN3, HIGH);
        digitalWrite(pin_IN4, LOW); 
    }
    else {
        digitalWrite(pin_IN3, LOW);
        digitalWrite(pin_IN4, HIGH); 
    }
    analogWrite(pin_EN34, abs(pwm_left));
}



















