#include <QTRSensors.h>

const unsigned int num_sensors = 5;
const unsigned int num_samples_per_sensor = 10;  // average analog samples per sensor reading
const byte pin_emitter_pin = 5;
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
    delay(5000);
    Serial.println("Begin test...");
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

void QTRARead() {
    pos = qtra.readLine(sensor_values); 
}


//============

bool isRoadLeft() {
    return (sensor_values[NUM_SENSORS-1] > 200);
}


//============

bool isRoadMiddle() {
    return (pos < (NUM_SENSORS-1)*1000*0.7 && pos > (NUM_SENSORS-1)*1000*0.3);
}


//============

bool isRoadRight() {
    return (sensor_values[0] > 200);
}
