#include <Messenger.h>
#include <Actuators.h>
#include <Sensors.h>

const byte pins_bluetooth[2] = {10, 11};  // RX, TX
Messenger* messenger = new Messenger(pins_bluetooth);

const byte pins_actuators[6] = {4,12,6,7,8,9};  // IN1, IN2, EN12, IN3, IN4, EN34
const float parameters_pid[4] = {65., 0.014, 0.008, 0.};  // target speed, kp, kd, ki
Actuators* actuators = new Actuators(pins_actuators, parameters_pid);

const byte pins_qta[6] = {0, 1, 2, 3, 4, 5};
const unsigned int parameters_qta[3] = {5, 6, 10};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
const unsigned char pin_sharp = 6;
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
    // test_8_1();
    test_8_2();
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

void align(bool clockwise) {
    if (clockwise)
        actuators->UpdateSpeed(60, -60);
    else
        actuators->UpdateSpeed(-60, 60);
    int error;
    do {
        delay(5);
        sensors->QTRARead();
    } while (!sensors->IsAligned());
    actuators->Stop();
}


//============

bool is_intersection() {
    is_road_left = sensors->IsRoadLeft();
    is_road_right = sensors->IsRoadRight();
    is_road_here = sensors->IsRoadHere();
    if (is_road_left || is_road_right || !is_road_here) {
        actuators->Stop();
        flicker_led(led_running, 20, 50);
        digitalWrite(led_running, HIGH);
        return true;
    } 
    return false;
}


//============

byte type_intersection() {
    one_inch();
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

void one_inch() {
    actuators->UpdateSpeed(60,60);
    delay(850);
    actuators->Stop();
}


//============

void left_hand_rule(byte type_intersection) {
    switch (type_intersection) {
        case 0:
            actuators->Turn(false, false);
            align(false);
            break;
        case 1:
            actuators->Turn(false, false);
            align(false);
            break;
        case 3: 
            actuators->Turn(false, false);
            align(false);
            break;
        case 4:
            actuators->Turn(false, false);
            align(false);
            break;
        case 5:
            actuators->Turn(true, false);
            align(true);
            break;
        case 6:
            actuators->Turn(true, true);
            align(true);
            break;
    }
    delay(200);
}
