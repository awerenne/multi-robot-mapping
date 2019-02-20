#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_


#include <Arduino.h>

// TODO: implement with anti-windup 


//============
class PIDController {
    private:
        float kp, kd, ki;
        float prev_error, acc_error;
        void updateError(float error);


    public: 
        PIDController(float kp, float kd, float ki);
        void setParameters(float kp, float kd, float ki);
        float update(float error);
        void reset();
        float getKp();
        float getKd();
        float getKi();
};


#endif  // PIDCONTROLLER_H_














