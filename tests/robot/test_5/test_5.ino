#include <Actuators.h>

const byte pin_interrupt_left = 3;
volatile unsigned long counter_left = 0;
const byte pin_interrupt_right = 2;
volatile unsigned long counter_right = 0;

unsigned long previous_time_left;
unsigned long previous_time_right;

const float alpha = 111.271;

const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  // IN1, IN2, EN12, IN3, IN4, EN34
Actuators* actuators = new Actuators(pins_actuators);


//============ 

void setup() {
    pinMode(pin_interrupt_left, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_left), update_counter_left, CHANGE);
    pinMode(pin_interrupt_right, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_right), update_counter_right, CHANGE);
    
    Serial.begin(9600);
    while (!Serial) continue;
    delay(5000);
    Serial.println("Begin test...");
}


//============

void loop() {
    test_5_1();
    // test_5_2();
    // test_5_3();
    exit(0);
}

void update_counter_left() { 
    counter_left++; 
}
void update_counter_right() { 
    counter_right++; 
}
