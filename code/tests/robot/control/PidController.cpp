#include "PIDController.h"


//============
PIDController::PIDController(float Kp, float Kd, float Ki) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    this->max = 50;
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
float PIDController::compute(float error) {
    unsigned long t = millis();
    float dt = (float) (t - this->prev_t);
    float kp = this->Kp;
    float kd = this->Kd / dt;
    float ki = this->Ki * dt;
    float iterm = this->antiWindup(error, ki);
    float corr = (kp * error) + (kd * this->prev_error) + iterm;
    this->update(error, t);
    return corr;
}


//============
float PIDController::antiWindup(float error, float ki) {
    float iterm = ki * this->acc_error;
    if (iterm > this->max) return this->max;
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
    this->prev_error = 0;
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
