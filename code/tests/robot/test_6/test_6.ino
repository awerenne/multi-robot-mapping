#include <SoftwareSerial.h>
#include <Messenger.h>
#include <Actuators.h>
#include <Sensors.h>

// Test two simple combinations of components (communication & motors, object sensors & motors)

const byte pins[2] = {10, 11}; // RX, TX
Messenger* messenger = new Messenger(pins);

const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  // IN1, IN2, EN12, IN3, IN4, EN34
Actuators* actuators = new Actuators(pins_actuators);

const byte pins_qta[6] = {0, 1, 2, 3, 4, 5};
const unsigned int parameters_qta[3] = {5, 6, 10};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
const byte pin_sharp = 6;
Sensors* sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);

const byte led_running = 13;
byte instruction = -1; // 0 stop, 1 straight, 2 turn left, 3 turn right, 4 turn back

//============

void setup() {
    pinMode(led_running, OUTPUT);
    digitalWrite(led_running, LOW);
    Serial.begin(9600);
    while(!Serial) continue;
    delay(5000);
    Serial.println("Begin test...");
}


//============

void loop() {
    test_6_1();
    // test_6_2();
    exit(0);
}
