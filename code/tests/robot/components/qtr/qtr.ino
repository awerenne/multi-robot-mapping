/*
    General description of tests.
*/


#include <QTRSensors.h>


const byte led_signal = 13;
const byte pin_emitter_pin = 5;

const unsigned int num_sensors = 6;
const unsigned int num_samples_per_sensor = 6;  // average analog samples per sensor reading
QTRSensorsAnalog qtra((unsigned char[]) {A0, A1, A2, A3, A4, A5}, num_sensors,
        num_samples_per_sensor);

const int f = 40;  
const int T = 30000;

unsigned int sensor_values[num_sensors];
unsigned int position_line;
int pos = 0;  


//============
void setup() {
    Serial.begin(9600);
    while (!Serial) continue;
    flicker_led(led_signal, 10, 200);
    digitalWrite(led_signal, HIGH);
    calibrate();
    digitalWrite(led_signal, LOW);
}


//============
void loop() {
    test(1);
    exit(0);
}
