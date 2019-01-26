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

const float parameters_pid[4] = {65., 0.014, 0.008, 0.};  // target speed, kp, kd, ki
const unsigned int parameters_qta[3] = {5, 6, 10};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
Messenger* messenger = new Messenger(pins_messenger);
Actuators* actuators = new Actuators(pins_actuators, parameters_pid);
Sensors* sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);
  
byte instruction = 0; // 0 stop, 1 straight, 2 turn left, 3 turn right, 4 turn back
bool is_road_left = false, is_road_right = false, is_road_here = false, is_road_front = false;


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

    unsigned int iteration = 0;
    float distance = 0;
    while (true) {
        bool new_msg = messenger->ReceiveMessage();  
        if (new_msg == true) {
            messenger->ParseInstruction();
            instruction = messenger->GetInstruction();
        }
    
        if (instruction == 0) {  // Stop 
            actuators->Stop();
            digitalWrite(led_signal, LOW);
        }
        else {  // Maze exploration
            digitalWrite(led_signal, HIGH);
            sensors->InitEncoders();
            sensors->QTRARead();
            int error = sensors->GetError();
            actuators->FollowLine(error);
            delay(10);
            
            // Compute distance
            distance += sensors->GetDistance();
            sensors->StopEncoders();

            // Check if intersection
            if ((iteration%2) == 0 && sensors->IsIntersection(actuators)) {
                sensors->InitEncoders();
                byte type_ = sensors->TypeIntersection(actuators);
                float distance_one_inch = sensors->GetDistance();
                sensors->StopEncoders();
                String information = String(type_) + ";" + String(distance);
                messenger->SendMessage(information);
                left_hand_rule(type_);
                distance += distance_one_inch;
            }

            // Check if obstacle
            else if ((iteration%5) == 0 && sensors->IsObstacle()) {
                actuators->Stop();
                String information = "8;" + String(distance);
                messenger->SendMessage(information);
                instruction = 0;
            }
        }
        iteration++;
    }
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
void left_hand_rule(byte type_intersection) {
    switch (type_intersection) {
        case 0:
            actuators->Turn(false, false, sensors);
            break;
        case 1:
            actuators->Turn(false, false, sensors);
            break;
        case 3: 
            actuators->Turn(false, false, sensors);
            break;
        case 4:
            actuators->Turn(false, false, sensors);
            break;
        case 5:
            actuators->Turn(true, false, sensors);
            break;
        case 6:
            actuators->Turn(true, true, sensors);
            break;
    }
    delay(200);
}


//============
void update_counter_left() { 
    sensors->IncrementLeft(); 
}


//============
void update_counter_right() { 
    sensors->IncrementRight(); 
}









