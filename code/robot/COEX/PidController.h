#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_


#include <Arduino.h>
#include <limits.h>


//============
class PIDController {
    public: 
        PIDController(double Kp, double Kd, double Ki);
        void setParameters(double Kp, double Kd, double Ki);
        double correction(double error);
        void reset();
        void setMin(double min_) { this->min_ = min_; }
        void setMax(double max_) { this->max_ = max_; }
        double getKp() { return Kp; }
        double getKd() { return Kd; }
        double getKi() { return Ki; }

    private:
        double Kp, Kd, Ki, prev_error, acc_error, min_, max_;
        unsigned long start_t, prev_t;

        void update(double error, unsigned long t);
        double antiWindup(double output);
        double antiDerivativeKick(double Dout);

};

#endif  // PIDCONTROLLER_H_














