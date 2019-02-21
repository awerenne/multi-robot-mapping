#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_


#include <Arduino.h>

// TODO: implement with anti-windup 


//============
class PIDController {
    private:
        float Kp, Kd, Ki;
        float prev_error, acc_error;
        float prev_t;
        float max;
        void update(float error, unsigned long t);
        float antiWindup(float error, float ki);

    public: 
        PIDController(float Kp, float Kd, float Ki);
        void setParameters(float Kp, float Kd, float Ki);
        float compute(float error);
        void reset();
        float getKp();
        float getKd();
        float getKi();
};


#endif  // PIDCONTROLLER_H_














