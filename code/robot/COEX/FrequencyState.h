#ifndef FREQUENCYSTATE_H_
#define FREQUENCYSTATE_H_


#include <Arduino.h>
#include <math.h>  // floor


//============
class FrequencyState {
    public: 
        FrequencyState(float frequency);
        bool isNewState();
        void setFrequency(float frequency);

    private:
        float frequency;
        unsigned long start_t;
        int prev_interval_number;
        
        int getInterval(unsigned long t);
        void reset();
};


#endif  // FREQUENCYSTATE_H_














