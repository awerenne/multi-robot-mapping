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

    this->previous_error = 0;
    this->accumulated_error = 0;
    if (parameters) {
        this->target_speed = parameters[0];
        this->kp = parameters[1];
        this->kd = parameters[2];
        this->ki = parameters[3];
    }
    else {
        this->target_speed = 65;
        this->kp = 0;
        this->kd = 0;
        this->ki = 0;
    }
    this->pid_value = 0;
    this->new_line = true;
}


//============
// PID controll and new line trick (explain more)
void Actuators::FollowLine(const int& error) { 
    this->accumulated_error += error;
    if (this->new_line) {
        this->pid_value = this->kp * error;
        this->new_line = false;
    }
    else
        this->pid_value = (this->kp * error) + (this->kd *
            (error - this->previous_error)) + (this->ki * this->accumulated_error);
    this->previous_error = error;
    float speed_left = 65 - pid_value;
    float speed_right = 65 + pid_value;
    this->UpdateSpeed(speed_left, speed_right); 
}


//============
void Actuators::OneInch() {
    this->UpdateSpeed(60, 60);
    delay(400);
    this->Stop();
    this->new_line = true;
}


//============
void Actuators::Stop() {
    digitalWrite(this->pin_IN1, HIGH);
    digitalWrite(this->pin_IN2, HIGH); 
    digitalWrite(this->pin_IN3, HIGH);
    digitalWrite(this->pin_IN4, HIGH); 
}


//============
void Actuators::Turn(const bool& clockwise, const bool& full, Sensors* sensors=NULL) {
    int n_quarters = full ? 2 : 1;
    for (int i = 0; i < n_quarters; ++i) {
        if (clockwise)
            this->UpdateSpeed(65, -65);
        else
            this->UpdateSpeed(-65, 65);
        delay(450);  // Time to do slightly less than half turn
    }
    if (sensors)
        this->TurnToLine(clockwise, sensors);
    this->Stop();
    delay(200);
}


//============
// Continue to turn until line is in centered (explain more)
void Actuators::TurnToLine(const bool& clockwise, Sensors* sensors) {
    if (clockwise)
        this->UpdateSpeed(58, -58);
    else
        this->UpdateSpeed(-58, 58);
    do {
        delay(5);
        sensors->QTRARead();
    } while (!sensors->IsAligned(clockwise));
}


//============
void Actuators::UpdatePWM(const int& pwm_left, const int& pwm_right) {
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


//============
void Actuators::UpdateSpeed(const float& speed_left, const float& speed_right) {
    int pwm_left = speed_left;
    int pwm_right;
    if (speed_right < 0)
        pwm_right = speed_right - 12;  // TODO: change to regression functions
    else 
        pwm_right = speed_right + 12;  // TODO: change to regression functions
    this->UpdatePWM(pwm_left, pwm_right);
}











