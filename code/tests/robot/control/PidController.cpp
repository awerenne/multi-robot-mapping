#include "PIDController.h"


//============
PIDController::PIDController(float Kp, float Kd, float Ki) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    this->max = 100;
    this->reset();
}


//============
void PIDController::setParameters(float Kp, float Kd, float Ki) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    this->reset();
}


//============
float PIDController::compute(float error, const byte& led = NULL) {
    unsigned long t = millis();
    float dt = (float) (t - this->prev_t);
    float kp = this->Kp;
    float kd = this->Kd / dt;
    float ki = this->Ki * dt;
    float iterm = this->antiWindup(error, ki, led);
    float dterm = (this->prev_error == this->myinf) ? 0 : (kd * this->prev_error);
    float corr = (kp * error) + dterm + iterm;
    this->update(error, t);
    return corr;
}


//============
float PIDController::antiWindup(float error, float ki, const byte& led = NULL) {
    float iterm = ki * this->acc_error;
    if (iterm > this->max) {
        if (led != NULL) digitalWrite(led, HIGH);
        return this->max;
    }
    if (led != NULL) digitalWrite(led, LOW);
    return iterm; 
}


//============
void PIDController::update(float error, unsigned long t) {
    this->prev_error = error;
    this->acc_error += error;
    this->prev_t = t;
}


//============
void PIDController::reset() {
    this->prev_t = millis();
    this->prev_error = this->myinf;
    this->acc_error = 0;
}


//============
float PIDController::getKp() {
    return this->Kp;
}


//============
float PIDController::getKd() {
    return this->Kd;
}


//============
float PIDController::getKi() {
    return this->Ki;
}
