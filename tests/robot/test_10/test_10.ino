#include <Messenger.h>
#include <Actuators.h>
#include <Sensors.h>

const byte pins_bluetooth[2] = {10, 11};  // RX, TX
Messenger* messenger = new Messenger(pins_bluetooth);

const byte pins_actuators[6] = {4,12,6,7,8,9};  // IN1, IN2, EN12, IN3, IN4, EN34
const float parameters_pid[4] = {65., 0.005, 0., 0.};  // target speed, kp, kd, ki
Actuators* actuators = new Actuators(pins_actuators, parameters_pid);

const byte pins_qta[6] = {0, 1, 2, 3, 4, 5};
const unsigned int parameters_qta[3] = {5, 6, 10};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
const unsigned char pin_sharp = 6;
Sensors* sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);

const byte led_running = 13;  // Used to signal when robot is calibrating IR sensors, and when the robot is running
byte instruction = -1; // 0 stop, 1 straight, 2 turn left, 3 turn right, 4 turn back
bool is_road_left = false, is_road_right = false, is_road_here = false, is_road_front = false;
float distance_step = 0, distance_total = 0;
const unsigned int delay_ = 20;
unsigned long int counter = 0;

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
    bool new_msg = messenger->ReceiveMessage();  
    if (new_msg == true) {
        messenger->ParseInstruction();
        instruction = messenger->GetInstruction();
        counter = 0;
    }

    if (instruction == 0) {  // Stop 
        actuators->Stop();
        digitalWrite(led_running, LOW);
    }
    else {  // Maze exploration
        digitalWrite(led_running, HIGH);
        sensors->QTRARead();
        int error = sensors->GetError();
        actuators->FollowLine(error);
        distance_step = actuators->GetDistanceStep();

        // Every second send distance step
        if ((counter % (1000/delay_)) == 0) 
            ;  // TODO send distance_step

        // Check if intersection
        if (is_intersection()) {
            actuators->Stop();
            flicker_led(led_running, 20, 50);
            digitalWrite(led_running, HIGH);
            byte type_ = type_intersection();
            // TODO send (instruction, distance) to master
            left_hand_rule(type_);
            distance = 0;
            counter = 0;
        }

        // Check if obstacle
        if (sensors->IsObstacle()) {
            actuators->Stop();
            instruction = 0;
        }
        delay(delay_);
    }

    counter++;
    distance_total += distance_step;
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

bool is_intersection() {
    is_road_left = sensors->IsRoadLeft();
    is_road_right = sensors->IsRoadRight();
    is_road_here = sensors->IsRoadHere();
    if (is_road_left || is_road_right)
        return true;
    return false;
}


//============

byte type_intersection() {
    actuators->OneInch();
    sensors->QTRARead();
    is_road_front = sensors->IsRoadHere();
    if (is_road_left && is_road_front && is_road_right)
        return 0;
    else if (is_road_left && is_road_front && !is_road_right)
        return 1;
    else if (!is_road_left && is_road_front && is_road_right)
        return 2;
    else if (is_road_left && !is_road_front && is_road_right)
        return 3;
    else if (is_road_left && !is_road_front && !is_road_right)
        return 4;
    else if (!is_road_left && !is_road_front && is_road_right)
        return 5;
    return 6;
}


//============

void left_hand_rule(byte type_intersection) {
    switch (type_intersection) {
        case 0:
            actuators->Turn(false, false, sensors);
            break;
        case 1:
            actuators->Turn(false, false, sensors);
            align(false);
            break;
        case 3: 
            actuators->Turn(false, false, sensors);
            align(false);
            break;
        case 4:
            actuators->Turn(false, false, sensors);
            align(false);
            break;
        case 5:
            actuators->Turn(true, false, sensors);
            align(true);
            break;
    }
    delay(200);
}
