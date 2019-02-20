/*
    Test autonomous exploration of maze by robot (Algorithm: left-hand rule)
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

// const float parameters_pid[4] = {65., 0.014, 0.008, 0.};  // target speed, kp, kd, ki
const float parameters_pid[4] = {71., 0.0163, 0.012, 0.};  // target speed (default 65), kp, kd, ki
const unsigned int parameters_qta[3] = {5, 6, 10};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
Messenger* messenger = new Messenger(pins_messenger);
Actuators* actuators = new Actuators(pins_actuators, parameters_pid);
Sensors* sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);

byte instruction = 0;
float distance = 0;

//============
void setup() {
    pinMode(pin_interrupt_left, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_left), update_counter_left, CHANGE);
    pinMode(pin_interrupt_right, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_right), update_counter_right, CHANGE);

    flicker_led(led_signal, 10, 200);
    digitalWrite(led_signal, LOW);   
}


//============
void loop() {   
    sensors->AutomaticCalibrate();
    while (true) {
        instruction = check_message(instruction);
        switch (instruction) {
            case 0: stop();
                    break;
            case 1: follow_line();
                    break;
            case 2: turn_left();
                    follow_line();
                    break;
            case 3: turn_right();
                    follow_line();
                    break;
            case 4: turn_back();
                    follow_line();
                    break;
            case 5: stop();
                    actuators->Accelerate();
                    break;
            default: stop();
        }
    }
}


//============
byte check_message(byte instruction) {
    bool new_msg = messenger->ReceiveMessage();  
    if (new_msg == true) {
        messenger->ParseInstruction();
        instruction = messenger->GetInstruction();
    }
    return instruction;
}


//============
void stop() {
    actuators->Stop();
    digitalWrite(led_signal, LOW);
    instruction = 0;
}


//============
void follow_line() {
    digitalWrite(led_signal, HIGH);
    distance = 0;
    instruction = 1;
    for (unsigned int i = 0; instruction == 1; i++) {
        follow_line_step();
        distance += sensors->GetDistance();
        if (i % 2 == 0 && sensors->IsIntersection(actuators)) {
            process_intersection(); 
            break;
        }
        if (i % 5 == 0 && sensors->IsObstacle(actuators)) {
            send_message(8, distance);
            break;
        }

        if (i % 10 == 0) {
            instruction = check_message(instruction);
            if (instruction != 1)
                return;
        }
    }
    instruction = 0;
    distance = 0;
}


//============
void turn_left() {
    actuators->Turn(false, false, sensors);
}


//============
void turn_right() {
    actuators->Turn(true, false, sensors);
}


//============
void turn_back() {
    actuators->Turn(false, true, sensors);
}


//============
void follow_line_step() {
    sensors->InitEncoders();
    sensors->QTRARead();
    actuators->FollowLine(sensors->GetError());
    delay(10);
}


//============
void process_intersection() {
    sensors->InitEncoders();
    byte type_intersection = sensors->TypeIntersection(actuators);
    float distance_one_inch = sensors->GetDistance();
    distance += distance_one_inch;
    send_message(type_intersection, distance);
}


//============
void send_message(byte type_intersection, float distance) {
    String information = String(type_intersection) + ";" + String(distance);
    messenger->SendMessage(information);
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
