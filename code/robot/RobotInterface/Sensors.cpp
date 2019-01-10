#include "Sensors.h"


//============

Sensors::Sensors(const byte* pins_qta, const unsigned int* parameters,
        const byte& pin_sharp) {
    this->qtra = new QTRSensorsAnalog(pins_qta, parameters[1],
        parameters[2]);
    this->num_sensors = parameters[1];
    this->sensor_values = (unsigned int*) malloc(this->num_sensors *
        sizeof(unsigned int));

    this->pin_sharp = pin_sharp;
    pinMode(this->pin_sharp, INPUT);
}

void Sensors::Calibrate() {
    // reads all sensors 10 times at 2500 us per read 
    for (int i = 0; i < 400; i++) this->qtra->calibrate();  
}
//============

void Sensors::QTRARead() {
    this->position = qtra->readLine(this->sensor_values);
}


//============

bool Sensors::IsRoadLeft() {
    return (this->sensor_values[this->num_sensors-1] > 800);
}


//============

bool Sensors::IsRoadRight() {
    return (this->sensor_values[0] > 800);
}


//============

bool Sensors::IsRoadHere() {
    for (int i = 1; i < this->num_sensors-1; i++) {
        if (sensor_values[i] > 500)
            return true;
    }
    return false;
}


//============

bool Sensors::IsAligned() {
    int mid = floor(((float) this->num_sensors)/2.);
    int sum = 0;
    sum = sensor_values[mid] + sensor_values[mid+1];
    if (sum > 1600)
        return true;
    return false;
}


//============

int Sensors::GetError() {
    int middle_position = (this->num_sensors-1)*1000/2;
    return (position-middle_position);
}


//============

bool Sensors::IsObstacle() {
    int value = analogRead(this->pin_sharp);
    float voltage = ((float) value)/1024.;
    if (voltage < 0.75) // Obstacle detected
        return true;
    return false;
}










