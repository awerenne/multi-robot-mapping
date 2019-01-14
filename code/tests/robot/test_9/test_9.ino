/*
    Test autonomous exploration of maze by robot (Algorithm: left-hand rule)
*/


#include <Messenger.h>
#include <Actuators.h>
#include <Sensors.h>


// TODO: encoder pins + descibe extensively used protocol

const byte pins_bluetooth[2] = {10, 11};  // RX, TX
Messenger* messenger = new Messenger(pins_bluetooth);

const byte pins_actuators[6] = {4,12,6,7,8,9};  // IN1, IN2, EN12, IN3, IN4, EN34
const float parameters_pid[4] = {65., 0.014, 0.008, 0.};  // target speed, kp, kd, ki
Actuators* actuators = new Actuators(pins_actuators, parameters_pid);

const byte pins_qta[6] = {0, 1, 2, 3, 4, 5};  // Analog --> Change to 14-21 range
const unsigned int parameters_qta[3] = {5, 6, 10};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
const byte pin_sharp = 6;
const byte pins_encoder[2] = {3, 2};
Sensors* sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);

const byte led_running = 13;  // Used to signal when robot is calibrating IR sensors, and when the robot is running
byte instruction = 0; // 0 stop, 1 straight, 2 turn left, 3 turn right, 4 turn back
bool is_road_left = false, is_road_right = false, is_road_here = false, is_road_front = false;


//============
void setup() {
    Serial.begin(9600);
    while(!Serial) continue; // Wait for init
    delay(5000);
    Serial.println("Begin test...");

    flicker_led(led_running, 10, 200);
    sensors->Calibrate();
    digitalWrite(led_running, LOW);   
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
            digitalWrite(led_running, LOW);
        }
        else {  // Maze exploration
            digitalWrite(led_running, HIGH);
            sensors->InitEncoders();
            sensors->QTRARead();
            int error = sensors->GetError();
            actuators->FollowLine(error);
            delay(10);
            
            // Compute distance
            distance += sensors->GetDistance();

            // Check if intersection
            if ((iteration%2) == 0 && sensors->IsIntersection(actuators)) {
                byte type_ = sensors->TypeIntersection(actuators);
                String msg = String(type_) + ";" + String(((int) distance));
                messenger->SendMessage(msg);
                left_hand_rule(type_);
                distance = 0;
            }

            // Check if obstacle
            else if ((iteration%5) == 0 && sensors->IsObstacle()) {
                actuators->Stop();
                String msg = "Y;" + String(((int) distance));
                messenger->SendMessage(msg);
                instruction = 0;
            }

            // Every now and then send update of travelled distance
            else if ((iteration%100) == 0) {
                String msg = "Z;" + String(((int) distance));
                messenger->SendMessage(msg);
                distance = 0;
            }
        }
        distance = 0;
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












