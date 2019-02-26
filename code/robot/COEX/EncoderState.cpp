#include "EncoderState.h"


//============
EncoderState::EncoderState() {
    reset();
}


//============
void EncoderState::reset() {
    prev_counter = counter;
    current_counter = prev_counter;
    prev_time = millis();
    current_time = prev_time;
}


//============
void EncoderState::newTimeStep() {
    prev_counter = current_counter;
    current_counter = counter;
    prev_time = current_time;
    current_time = millis();
}


//============
void EncoderState::incrementCounter() {
    counter++;
}


//============
float EncoderState::deltaTime() {
    return current_time - prev_time;
}


//============
float EncoderState::deltaCounter() {
    return current_counter - prev_counter;
}



















