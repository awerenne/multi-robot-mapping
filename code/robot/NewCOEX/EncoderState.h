#ifndef ENCODERSTATE_H_
#define ENCODERSTATE_H_


#include <Arduino.h>


//============
class EncoderState {
    public: 
        EncoderState();
        void reset();
        void newTimeStep();
        void incrementCounter();
        float deltaTime();
        float deltaCounter();

    private:
        volatile unsigned long counter;
        unsigned long prev_counter, current_counter, prev_time, current_time;
    
};


#endif  // ENCODERSTATE_H_














