#include "Sensors.h"


Sensors::Sensors(QTRSensorsRC* qtrrc, int num_sensors, char pin_sharp) {
    this->qtrrc = qtrrc;
    this->num_sensors = num_sensors;
    this->sensor_values = (unsigned int*) malloc(this->num_sensors *
        sizeof(unsigned int));
    this->pin_sharp = pin_sharp;
}


void Sensors::QTRRead() {
    this->position = qtrrc->readLine(this->sensor_values);
}


bool Sensors::IsRoadLeft() {
    return (this->sensor_values[0] > 200);
}


bool Sensors::IsRoadRight() {
    return (this->sensor_values[this->num_sensors-1] > 200);
}


bool Sensors::IsRoadMiddle() {
    return (this->position < (this->num_sensors-1)*1000*0.8
        && this->position > (this->num_sensors-1)*1000*0.2);
}


int Sensors::GetError() {
    int middle_position = (this->num_sensors-1)*1000/2;
    return (this->position - middle_position);
}


float Sensors::SharpRead() {
    int analog_value = analogRead(pin_sharp);  
    // Analog value is an integer between 0 and 1023 mapping the output voltage 
    // from 0 to VCC

    // If VCC = 5V (less precise!)
    // Map to corresponding output
    float voltage = ((float) analog_value) / 1024. * 5.;

    if (voltage < 0.3) // Out of range (too close or too far)
        return -1.;
    float a, b;  // Find regression function by experiments
    return (a*voltage + b);  // distance to object
}
