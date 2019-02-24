/*
    Test two simple combinations of components (communication & motors, object sensors & motors)
*/


#include <Coex.h>


const byte pins_messenger[2] = {10, 11}; 
const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  
const byte pins_qta[6] = {A0, A1, A2, A3, A4, A5};  
const byte pin_sharp = 6;
const byte led_signal = 13;
const unsigned int pin_emitter = 5, n_sensors = 6, n_samples = 4;
const unsigned int parameters_qta[3] = {pin_emitter, n_sensors, n_samples}; 

Coex* coex = new Coex(pins_qta, parameters_qta, pin_sharp, pins_actuators,
        pins_messenger);

bool dynamic_state = false;
byte instruction = -1; 
unsigned int delay_ = 5;
FrequencyState* freq_reception = new FrequencyState(10);
FrequencyState* freq_obstacle = new FrequencyState(10);
float progress_speed = 0, target_speed = 0;
Accelerator* acc = new Accelerator(progress_speed, target_speed, 1);


//============
void setup() {
    Serial.begin(9600);
    while(!Serial) continue;
    flicker_led(led_signal, 10, 500);
}


//============
void loop() {
    if (freq_reception->isNewState()) {
        float new_target_speed = coex->receive_msg_manual(instruction);
        if (target_speed != new_target_speed) {
            target_speed = new_target_speed;
            acc->reset(progress_speed, target_speed, 500);
            acc->start();
        }
        switch (instruction) {
            case 0:  
                dynamic_state = false;
                coex->stop();
                progress_speed = 0;
                target_speed = 0;
                acc->reset(progress_speed, target_speed, 1);
                break;
            case 1: 
                dynamic_state = true;
                break;
            case 2: 
                bool clockwise = true;
                coex->turn90(clockwise);
                break;
            case 3:
                bool clockwise = false;
                coex->turn90(clockwise);
                break;
            case 4:
                bool clockwise = true;
                coex->turn180(clockwise);
                break;
            case 5:
                bool clockwise = false;
                coex->turn180(clockwise);  
                break;  
        }
    }

    if (dynamic_state) {
        acc->accelerate(progress_speed);
        coex->forward(progress_speed);
    }
    delay(delay_);
}








































