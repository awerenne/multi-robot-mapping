#ifndef ACCELERATOR_H_
#define ACCELERATOR_H_


#include <Arduino.h>


//============
class Accelerator {
    public: 
        Accelerator(float alpha=0);
        void start(float& progress_speed, const float& target_speed,
                const float& T);
        void stop(float& progress_speed);
        void accelerate(float& progress_speed);  
        bool saturation(const float& progress_speed);  
        bool isRunning() { return this->is_running; } 

    private:
        float min_speed, target_speed, alpha, beta, A, B, T, prev_t, current_t;
        unsigned long start_t;
        bool is_running;
        
        void constantAcceleration(float& progress_speed);
        void smoothAcceleration(float& progress_speed);
        float uniformProfile();
        float trapezoidProfile(float t);
        float integral(float t);

};


#endif  // ACCELERATOR_H_
