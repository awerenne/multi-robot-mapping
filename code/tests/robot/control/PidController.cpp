#include "PIDController.h"


//============
PIDController::PIDController(float kp, float kd, float ki) {
    this->kp = kp;
    this->kd = kd;
    this->ki = ki;
    this->prev_error = 0;
    this->acc_error = 0;
}


//============
void PIDController::setParameters(float kp, float kd, float ki) {
    this->kp = kp;
    this->kd = kd;
    this->ki = ki;
}


//============
float PIDController::update(float error) {
    float value = (this->kp * error) + (this->kd * this->prev_error) + 
            (this->ki * this->acc_error);
    this->updateError(error);
    return value;
}


//============
void PIDController::updateError(float error) {
    this->prev_error = error;
    this->acc_error += error;
}


//============
void PIDController::reset() {
    this->prev_error = 0;
    this->acc_error = 0;
}


//============
float PIDController::getKp() {
    return this->kp;
}


//============
float PIDController::getKd() {
    return this->kd;
}


//============
float PIDController::getKi() {
    return this->ki;
}

























