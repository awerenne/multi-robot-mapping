/*
    Control of speed and line-following.
*/


#include <Messenger.h>
#include <Actuators.h>
#include <Sensors.h>
#include <PidController.h>
#include <FrequencyState.h>
#include <Accelerator.h>
#include <math.h>


const byte pins_messenger[2] = {10, 11}; // RX, TX
const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  // IN1, IN2, EN12, IN3, IN4, EN34
const byte pins_qta[6] = {A0, A1, A2, A3, A4, A5};  
const byte pin_sharp = 6;
const byte led_signal = 13;
const byte pin_interrupt_right = 2;
const byte pin_interrupt_left = 3;

Messenger* messenger = new Messenger(pins_messenger, 9600);
Actuators* actuators = new Actuators(pins_actuators);
const unsigned int parameters_qta[3] = {5, 6, 8};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
Sensors* sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);

PIDController* pid_speed = new PIDController(0,0,0);
PIDController* pid_forward = new PIDController(0,0,0);
PIDController* pid_line = new PIDController(0,0,0);
int instruction = -1;

//============
void setup() {
    pinMode(pin_interrupt_left, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_left),
            update_counter_left, CHANGE);
    pinMode(pin_interrupt_right, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_right),
            update_counter_right, CHANGE);
            
    Serial.begin(9600);
    flicker_led(led_signal, 10, 200);
    digitalWrite(led_signal, LOW); 
    while(!Serial) continue; 
    digitalWrite(led_signal, LOW);   
}


//============
void loop() {
    test(6);
    exit(0);
}


//============
void update_counter_left() { sensors->incrementLeft(); }
void update_counter_right() { sensors->incrementRight(); }
