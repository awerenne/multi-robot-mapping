/*
    General description of tests.
*/


#include <QTRSensors.h>


const byte led_signal = 13;
const byte pin_emitter_pin = 5;

const unsigned int num_sensors = 5;
const unsigned int num_samples_per_sensor = 10;  // average analog samples per sensor reading
QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4, 5}, num_sensors, num_samples_per_sensor);

const int f = 50;  // frequency
const bool light_on = true;
const float h = 10;  // height
const int T = 40000;
const int dimming_level = 0;

unsigned int sensor_values[num_sensors];
unsigned int position_line;
int pos = 0;  // line position


//============
void setup() {
    Serial.begin(9600);
    while (!Serial) continue;
    flicker_led(led_signal, 10, 500);

    Serial.println("Start Calibration");
    // Reads all sensors 10 times at 2500 us per read 
    for (int i = 0; i < 400; i++) qtra.calibrate(); 
    Serial.println("End Calibration");
}


//============
void loop() {
    test_3_1();
    // test_3_2();
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














