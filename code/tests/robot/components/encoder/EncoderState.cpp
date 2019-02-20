#include "EncoderState.h"


//============
EncoderState::EncoderState() {
    this->resetCounter();
}


//============
void EncoderState::resetCounter() {
    this->counter = 0;
    this->prev_counter = 0;
    this->prev_time = millis();
}


//============
void EncoderState::newTimeStep() {
    this->prev_counter = this->current_counter;
    this->current_counter = this->counter;
    this->prev_time = this->current_time;
    this->current_time = millis();
}


//============
void EncoderState::incrementCounter() {
    this->counter++;
}


//============
float EncoderState::deltaTime() {
    return (float) (this->current_time - this->prev_time);
}


//============
float EncoderState::deltaCounter() {
    return (float) (this->current_counter - this->prev_counter);
}

















