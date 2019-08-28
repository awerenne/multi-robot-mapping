#include <Anomalies.h>
#include <Sensors.h>
#include <Accelerator.h>
#include <FrequencyState.h>
#include <Coex.h>
#include <EncoderState.h>
#include <Actuators.h>
#include <PidController.h>
#include <Messenger.h>

const byte pin_interrupt_left = 3;
const byte pin_interrupt_right = 2;
const byte pins_messenger[2] = {10, 11}; // RX, TX
const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  // IN1, IN2, EN12, IN3, IN4, EN34
const byte pins_qta[6] = {A0, A1, A2, A3, A4, A5};  // Analog 
const byte pin_sharp = A6;
const byte led_signal = 13;
const int baud_rate = 9600;
const unsigned int pin_emitter = 5, n_sensors = 6, n_samples = 4;
const unsigned int parameters_qta[3] = {pin_emitter, n_sensors, n_samples}; 
Coex* coex = new Coex(pins_messenger, pins_actuators, pins_qta, pin_sharp,
            baud_rate, parameters_qta);

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
    test(3);
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
void update_counter_left() { coex->getSensors()->incrementLeft(); }
void update_counter_right() { coex->getSensors()->incrementRight(); }
