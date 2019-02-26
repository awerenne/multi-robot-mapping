#include "PIDController.h"


//============
PIDController::PIDController(double Kp, double Kd, double Ki) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    min_ = 0;
    max_ = 0;
    this->reset();
}


//============
void PIDController::setParameters(double Kp, double Kd, double Ki) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    this->reset();
}


//============
double PIDController::correction(double error) {
    unsigned long t = millis();
    double delta_t = (double) (t - prev_t);
    acc_error += error * delta_t;

    double Pout = Kp * error;
    double Dout = antiDerivativeKick((error-prev_error)/delta_t);
    double Iout = Ki * acc_error;
    
    prev_error = error;
    prev_t = t;
    return antiWindup(Pout + Dout + Iout);
}


//============
double PIDController::antiWindup(double output) {
    if (max_ != 0 && output > max_) return max_;
    if (min_ != 0 && output < min_) return min_;
    return output; 
}


//============
double PIDController::antiDerivativeKick(double Dout) {
    if (millis() < start_t + 500) return 0;
    return Dout;
}


//============
void PIDController::reset() {
    start_t = millis();
    prev_t = millis();
    prev_error = 0;
    acc_error = 0;
}















