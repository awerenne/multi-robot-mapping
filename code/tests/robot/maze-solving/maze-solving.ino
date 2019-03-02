#include <Actuators.h>
#include <Sensors.h>
#include <FrequencyState.h>
#include <Accelerator.h>
#include <Coex.h>
#include <PidController.h>
#include <Anomalies.h>


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
byte instruction; 
PIDController* pid_speed = new PIDController(12, 0, 0.022);
PIDController* pid_forward = new PIDController(15, 0.07, 0.065);

//============
void setup() {
    pinMode(pin_interrupt_left, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_left), update_counter_left, CHANGE);
    pinMode(pin_interrupt_right, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_right), update_counter_right, CHANGE);

    Serial.begin(9600);
    while(!Serial) continue;
    flicker_led(led_signal, 10, 200);
    digitalWrite(led_signal, LOW);   
}


//============
void loop() {
    test(8);
    exit(0);
}


//============
void update_counter_left() { coex->getSensors()->incrementLeft(); }
void update_counter_right() { coex->getSensors()->incrementRight(); }
