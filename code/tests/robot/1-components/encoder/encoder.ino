/*
  General description of tests
*/


#include <Actuators.h>
#include <SoftwareSerial.h>
#include <EncoderState.h>


const byte led_signal = 13;
const byte pin_interrupt_right = 2;
const byte pin_interrupt_left = 3;
const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  // IN1, IN2, EN12, IN3, IN4, EN34
Actuators* actuators = new Actuators(pins_actuators);
EncoderState* encoder_left = new EncoderState();
EncoderState* encoder_right = new EncoderState();
SoftwareSerial* bluetooth = new SoftwareSerial(10, 11);

//============ 
void setup() {
    digitalWrite(led_signal, HIGH);
    pinMode(pin_interrupt_left, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_left),
            update_counter_left, CHANGE);
    pinMode(pin_interrupt_right, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_right),
            update_counter_right, CHANGE);
    
    Serial.begin(9600);
    while (!Serial) continue;
      
    while (!bluetooth) continue;  
    bluetooth->begin(9600);

    digitalWrite(led_signal, LOW);
    delay(5000);
}


//============
void loop() {
    test(2);
    exit(0);
}
