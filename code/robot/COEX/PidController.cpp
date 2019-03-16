#include "PIDController.h"


//============
PIDController::PIDController(double Kp, double Kd, double Ki) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    min_ = -255;
    max_ = 255;
    Imin = -255;
    Imax = 255;
    zeta = 0;
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
    error = errorWeighting(error, t);
    acc_error += error * delta_t;

    double Pout = Kp * error;
    double Dout = (error-prev_error)/delta_t;
    double Iout = antiWindup(Ki * acc_error);
    
    prev_error = error;
    prev_t = t;
    return limitOutput(Pout + Dout + Iout);
}


//============
double PIDController::errorWeighting(double err, unsigned long t) {
    if (zeta == 0) return err;
    float x = (float) (t - start_t);
    return tanh(zeta * x / 1000.) * err;
}


//============
double PIDController::limitOutput(double output) {
    if (output > max_) return max_;
    if (output < min_) return min_;
    return output; 
}


//============
double PIDController::antiWindup(double Iout) {
    if (Iout > Imax) return Imax;
    if (Iout < Imin) return Imin;
    return Iout; 
}


//============
double PIDController::antiDerivativeKick(double Dout) {
    if (millis() < start_t + 1000) return 0;
    return Dout;
}


//============
void PIDController::reset() {
    start_t = millis();
    prev_t = millis();
    prev_error = 0;
    acc_error = 0;
}















