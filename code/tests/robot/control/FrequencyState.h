#ifndef FREQUENCYSTATE_H_
#define FREQUENCYSTATE_H_


#include <Arduino.h>
#include <math.h>  // floor


//============
class FrequencyState {
    private:
        float frequency;
        unsigned long start_t;
        int prev_interval_number;
        int getInterval(unsigned long t);
        void reset();

    public: 
        FrequencyState(float frequency);
        bool isNewState();
        void setFrequency(float frequency);
};


#endif  // FREQUENCYSTATE_H_














