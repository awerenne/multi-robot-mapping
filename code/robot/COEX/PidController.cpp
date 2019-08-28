#include "PIDController.h"


//============
PIDControllerSpeed::PIDControllerSpeed(double Kp, double Kd, double Ki) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    min_ = -255;
    max_ = 255;
    Imin = -255;
    Imax = 255;
    this->reset();
}


//============
void PIDControllerSpeed::setParameters(double Kp, double Kd, double Ki) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    this->reset();
}


//============
double PIDControllerSpeed::correction(double error) {
    unsigned long t = millis();
    double delta_t = (double) (t - prev_t);
    acc_error += error * delta_t;

    double Pout = Kp * error;
    double Dout = (error-prev_error)/delta_t;
    double Iout = antiWindup(Ki * acc_error);
    
    prev_error = error;
    prev_t = t;
    return limitOutput(Pout + Dout + Iout);
}


//============
double PIDControllerSpeed::limitOutput(double output) {
    if (output > max_) return max_;
    if (output < min_) return min_;
    return output; 
}


//============
double PIDControllerSpeed::antiWindup(double Iout) {
    if (Iout > Imax) return Imax;
    if (Iout < Imin) return Imin;
    return Iout; 
}


//============
void PIDControllerSpeed::reset() {
    start_t = millis();
    prev_t = millis();
    prev_error = 0;
    acc_error = 0;
}




//============
//============
//============
//============






//============
PIDControllerLine::PIDControllerLine(double Kp, double Kd, double Ki) {
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
void PIDControllerLine::setParameters(double Kp, double Kd, double Ki) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    this->reset();
}


//============
double PIDControllerLine::correction(double x, double position_setpoint, double speed_setpoint) {
    unsigned long t = millis();
    double delta_t = (double) (t - prev_t);
    double error = floorf(((x - position_setpoint)/1250.)*100)/100;

    if (tanh(zeta * speed_setpoint) >= 0.85) 
        acc_error += error * delta_t;
    else
        acc_error = 0;

    double Pout = Kp * error; 
    double Dout = Kd * (x - prev_x)/delta_t; 
    double Iout = antiWindup(Ki * acc_error); 
    
    prev_x = x;
    prev_t = t;
    return abs(tanh(zeta * speed_setpoint)) * limitOutput(Pout + Dout + Iout);
}


//============
double PIDControllerLine::limitOutput(double output) {
    if (output > max_) return max_;
    if (output < min_) return min_;
    return output; 
}


//============
double PIDControllerLine::antiWindup(double Iout) {
    if (Iout > Imax) return Imax;
    if (Iout < Imin) return Imin;
    return Iout; 
}


//============
void PIDControllerLine::reset() {
    start_t = millis();
    prev_t = millis();
    prev_x = 0;
    acc_error = 0;
}












