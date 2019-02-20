#ifndef ENCODERSTATE_H_
#define ENCODERSTATE_H_


#include <Arduino.h>


//============
class EncoderState {
    private:
        volatile unsigned long counter;
        unsigned long prev_counter, current_counter;
        unsigned long prev_time, current_time;

    public: 
        EncoderState();
        void resetCounter();
        void newTimeStep();
        void incrementCounter();
        float deltaTime();
        float deltaCounter();
};



#endif  // ENCODERSTATE_H_


























