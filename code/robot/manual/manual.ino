/*
    ...
*/


#include <Coex.h>


const byte pins_messenger[2] = {10, 11}; 
const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  
const byte pins_qta[6] = {A0, A1, A2, A3, A4, A5};  
const byte pin_sharp = 6;
const byte led_signal = 13;
const byte pin_interrupt_right = 2;
const byte pin_interrupt_left = 3;
const int baud_rate = 9600;
const unsigned int pin_emitter = 5, n_sensors = 6, n_samples = 4;
const unsigned int parameters_qta[3] = {pin_emitter, n_sensors, n_samples}; 

Coex* coex = new Coex(pins_messenger, pins_actuators, pins_qta, pin_sharp,
            baud_rate, parameters_qta);
bool dynamic_state = false;
byte instruction; 
float target_speed = 6;

//============
void setup() {
    pinMode(pin_interrupt_left, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_left),
            update_counter_left, CHANGE);
    pinMode(pin_interrupt_right, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_right),
            update_counter_right, CHANGE);

    Serial.begin(9600);
    while(!Serial) continue;
    flicker_led(led_signal, 10, 500);
}


//============
void loop() {
    if (coex->availableMsg()) {
        coex->readMsg();
        int instruction = coex->getMsgInstruction();
        Serial.println(instruction);
        switch (instruction) {
            case 0: 
                coex->stop(); 
                dynamic_state = false;
                break;
            case 1: 
                dynamic_state = true; 
                coex->newForward(target_speed);
                break;
            case 2: coex->turn90(true, target_speed); break;
            case 3: coex->turn90(false, target_speed); break;
            case 4: coex->turn180(true, target_speed); break;
        }
    }
    if (dynamic_state) coex->forward();
    delay(5);
}


//============
void flicker_led(byte led, unsigned int n, unsigned int delay_) {
    for (int i = 0; i < n; i++) {
        digitalWrite(led, LOW);
        delay(delay_);
        digitalWrite(led, HIGH);
        delay(delay_);
    }
}


//============
void update_counter_left() { sensors->incrementLeft(); }
void update_counter_right() { sensors->incrementRight(); }




