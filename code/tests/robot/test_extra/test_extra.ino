/*
    Description.
*/


#include <Messenger.h>
#include <Actuators.h>
#include <Sensors.h>


const byte pin_interrupt_left = 3;
const byte pin_interrupt_right = 2;
const byte pins_messenger[2] = {10, 11}; // RX, TX
const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  // IN1, IN2, EN12, IN3, IN4, EN34
const byte pins_qta[6] = {0, 1, 2, 3, 4, 5};  // Analog 
const byte pin_sharp = 6;
const byte led_signal = 13;

Messenger* messenger = new Messenger(pins_messenger);
Actuators* actuators = new Actuators(pins_actuators);
const unsigned int parameters_qta[3] = {5, 6, 10};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
Sensors* sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);

byte instruction = -1; // 0 stop, 1 straight, 2 turn left, 3 turn right, 4 turn back
bool is_road_left = false, is_road_right = false, is_road_here = false, is_road_front = false;


//============
void setup() {
    pinMode(pin_interrupt_left, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_left), update_counter_left, CHANGE);
    pinMode(pin_interrupt_right, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_right), update_counter_right, CHANGE);

    Serial.begin(9600);
    while(!Serial) continue; // Wait for init
    flicker_led(led_signal, 10, 500);
}


//============
void loop() {
    test_extra_1();    
    // test_extra_2();
    // test_extra_3();
    exit(0);
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
void update_counter_left() { 
    sensors->IncrementLeft(); 
}


//============
void update_counter_right() { 
    sensors->IncrementRight(); 
}











