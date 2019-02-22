#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_


#include <Arduino.h>


//============
class PIDController {
    private:
        float myinf = 100000;
        float Kp, Kd, Ki;
        float prev_error, acc_error, max;
        unsigned long prev_t;
        void update(float error, unsigned long t);
        float antiWindup(float error, float ki, const byte& pin_led = NULL);

    public: 
        PIDController(float Kp, float Kd, float Ki);
        void setParameters(float Kp, float Kd, float Ki);
        float compute(float error, const byte& pin_led = NULL);
        void reset();
        float getKp();
        float getKd();
        float getKi();
};


#endif  // PIDCONTROLLER_H_














