#include "FrequencyState.h"


//============
FrequencyState::FrequencyState(float frequency) {
    this->frequency = frequency;
    reset();
}


//============
void FrequencyState::reset() {
    prev_interval_number = 0;
    start_t = millis();
}


//============
bool FrequencyState::isNewState() {
    int interval_number = getInterval(millis());
    if (interval_number == prev_interval_number) return false;
    prev_interval_number = interval_number;
    return true;
}


//============
int FrequencyState::getInterval(unsigned long t) {
    float delta_t = (float) (t - start_t);
    float time_per_interval = 1000. / frequency;  
    return floor(delta_t/time_per_interval);
}


//============
void FrequencyState::setFrequency(float frequency) {
    if (frequency == this->frequency) return;
    this->frequency = frequency;
    reset();
}



























