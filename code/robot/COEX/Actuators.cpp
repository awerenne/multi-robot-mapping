#include "Actuators.h"


//============
Actuators::Actuators(const byte *pins, const float* parameters=NULL) { 
    this->pin_IN1 = pins[0]; 
    this->pin_IN2 = pins[1];
    this->pin_EN12 = pins[2];

    this->pin_IN3 = pins[3];
    this->pin_IN4 = pins[4];
    this->pin_EN34 = pins[5];

    pinMode(this->pin_EN12, OUTPUT);
    pinMode(this->pin_IN1, OUTPUT);
    pinMode(this->pin_IN2, OUTPUT);

    pinMode(this->pin_EN34, OUTPUT);
    pinMode(this->pin_IN3, OUTPUT);
    pinMode(this->pin_IN4, OUTPUT);

    digitalWrite(this->pin_IN1, HIGH);
    digitalWrite(this->pin_IN2, HIGH);
    digitalWrite(this->pin_IN3, HIGH);
    digitalWrite(this->pin_IN4, HIGH);
}



//============
void Actuators::stop() {
    digitalWrite(this->pin_IN1, HIGH);
    digitalWrite(this->pin_IN2, HIGH); 
    digitalWrite(this->pin_IN3, HIGH);
    digitalWrite(this->pin_IN4, HIGH); 
}



//============
void Actuators::updatePWM(const int& pwm_left, const int& pwm_right) {
    if (pwm_right >= 0) {
        digitalWrite(this->pin_IN1, HIGH);
        digitalWrite(this->pin_IN2, LOW); 
    }
    else {
        digitalWrite(this->pin_IN1, LOW);
        digitalWrite(this->pin_IN2, HIGH); 
    }
    analogWrite(this->pin_EN12, abs(pwm_right));

    if (pwm_left >= 0) {
        digitalWrite(this->pin_IN3, HIGH);
        digitalWrite(this->pin_IN4, LOW); 
    }
    else {
        digitalWrite(this->pin_IN3, LOW);
        digitalWrite(this->pin_IN4, HIGH); 
    }
    analogWrite(this->pin_EN34, abs(pwm_left));
}



















