#include "FrequencyState.h"


//============
FrequencyState::FrequencyState(float frequency) {
    /* frequency is in Hz */
    this->frequency = frequency;
    this->reset();
}


//============
void FrequencyState::reset() {
    this->prev_interval_number = 0;
    this->start_t = millis();
}


//============
bool FrequencyState::isNewState() {
    unsigned long t = millis();
    int interval_number = this->getInterval(t);
    if (interval_number == this->prev_interval_number) return false;
    this->prev_interval_number = interval_number;
    return true;
}


//============
int FrequencyState::getInterval(unsigned long t) {
    float delta_t = (float) (t - this->start_t);
    float time_per_state = 1000. / frequency;  // in milliseconds
    return floor(delta_t/time_per_state);
}


//============
void FrequencyState::setFrequency(float frequency) {
    if (frequency == this->frequency) return;
    this->frequency = frequency;
    this->reset();
}



























