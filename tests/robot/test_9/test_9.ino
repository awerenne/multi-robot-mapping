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
    test_9_1();
    // test_9_2();
    // test_9_3();
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
