#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_


#include <Arduino.h>
#include <limits.h>
#include <math.h>


//============
class PIDController {
    public: 
        PIDController(double Kp, double Kd, double Ki);
        void setParameters(double Kp, double Kd, double Ki);
        double correction(double error);
        void reset();
        void setMin(double min_) { this->min_ = min_; }
        void setMax(double max_) { this->max_ = max_; }
        void setWindup(double Imin, double Imax) { this->Imin = Imin; this->Imax = Imax; }
        void setZeta(double zeta) { this->zeta = zeta; }
        double getKp() { return Kp; }
        double getKd() { return Kd; }
        double getKi() { return Ki; }

    private:
        double Kp, Kd, Ki, prev_error, acc_error, min_, max_, Imin, Imax, zeta, Z;
        unsigned long start_t, prev_t;

        void update(double error, unsigned long t);
        double antiWindup(double output);
        double antiDerivativeKick(double Dout);
        double limitOutput(double output);
        double errorWeighting(double err, unsigned long t);

};

#endif  // PIDCONTROLLER_H_














