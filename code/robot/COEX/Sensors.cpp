#include "Sensors.h"


//============
Sensors::Sensors(const byte* pins_qta, const unsigned int* parameters,
        const byte& pin_sharp) {
    // TODO assert size
    this->qtra = new QTRSensorsAnalog(pins_qta, parameters[1],
        parameters[2]);
    this->num_sensors = parameters[1];
    this->sensor_values = (unsigned int*) malloc(this->num_sensors *
        sizeof(unsigned int));

    this->pin_sharp = pin_sharp;
    pinMode(this->pin_sharp, INPUT);

    // TODO assert size  // To clean - here and in code
    this->counter_active = false;
    this->counter_left = 0;
    this->counter_right = 0;
}


//============
void Sensors::Calibrate() {
    // reads all sensors 10 times at 2500 us per read 
    for (int i = 0; i < 400; i++) this->qtra->calibrate();  
}


//============
void Sensors::AutomaticCalibrate() {
    // Determine if we are in the dark
    bool dark = false;
    qtra->read(sensor_values, QTR_EMITTERS_OFF); 
    for (int i = 0; i < this->num_sensors && !dark; i++) {
        if (this->sensor_values[i] > 300)
            dark = true;
    }
    
    unsigned int calib_min_dark[6] = {270,124,126,112,117,260};  // TODO replace to regression
    unsigned int calib_max_dark[6] = {720,615,620,590,595,710};  // TODO replace to regression

    unsigned int calib_min_light[6] = {120,110,110,110,110,175};  // TODO replace to regression
    unsigned int calib_max_light[6] = {720,615,620,590,595,710};  // TODO replace to regression

    this->qtra->calibratedMinimumOn = (unsigned int*) malloc(this->num_sensors *
        sizeof(unsigned int));
    this->qtra->calibratedMaximumOn = (unsigned int*) malloc(this->num_sensors *
        sizeof(unsigned int));
    for (int i = 0; i < this->num_sensors; i++) {
        if (dark) {
            this->qtra->calibratedMinimumOn[i] = calib_min_dark[i];
            this->qtra->calibratedMaximumOn[i] = calib_max_dark[i];
        }
        else {
            this->qtra->calibratedMinimumOn[i] = calib_min_light[i];
            this->qtra->calibratedMaximumOn[i] = calib_max_light[i];
        }
    }    
}



//============
void Sensors::QTRARead() {
    this->position = this->qtra->readLine(this->sensor_values);
}


//============
QTRSensorsAnalog* Sensors::GetQTRObject() { 
    return this->qtra;
}


//============
unsigned int* Sensors::GetQTRValues() {
    return this->sensor_values;
}


//============
unsigned int Sensors::GetQTRNumberSensors() {
    return this->num_sensors;
}


//============
bool Sensors::IsRoadLeft() {  
    return (this->sensor_values[this->num_sensors-1] > 750);
}


//============
bool Sensors::IsRoadRight() {  
    return (this->sensor_values[0] > 750);
}


//============
bool Sensors::IsRoadHere() {  
    return (this->sensor_values[1]+this->sensor_values[2] > 1500 || 
            this->sensor_values[2]+this->sensor_values[3] > 1500 ||
            this->sensor_values[3]+this->sensor_values[4] > 1500 ||
            this->sensor_values[1] > 750 || 
            this->sensor_values[2] > 750 ||
            this->sensor_values[3] > 750 ||
            this->sensor_values[4] > 750);
}


//============
bool Sensors::IsAligned(bool clockwise) {  // To clean - here and in code
    // int mid = floor(((float) this->num_sensors)/2.); TO CHANGE
    int sum = this->sensor_values[2] + this->sensor_values[3];
    if (clockwise && this->sensor_values[2] > 850)  // Give some margin to stop
        return true;
    if (!clockwise && this->sensor_values[3] > 850) // Give some margin to stop
        return true;
    // To make sure it does not pass 
    if (!clockwise && this->sensor_values[2] > 600)  // Give some margin to stop
        return true;
    if (clockwise && this->sensor_values[3] > 600) // Give some margin to stop
        return true;
    return false;
}


//============
int Sensors::GetError() {
    int middle_position = (this->num_sensors-1)*1000/2;
    return (position-middle_position);
}


//============
bool Sensors::IsObstacle(Actuators* actuators) {
    int value = analogRead(this->pin_sharp);
    float voltage = ((float) value)/1024.;
    if (voltage < 0.75) {  // Obstacle detected
        actuators->Stop();
        delay(1000);
        return true;
    }
    return false;
}


//============
void Sensors::InitEncoders() {  
    this->counter_active = true;
    this->counter_left = 0;
    this->counter_right = 0;
    this->previous_time = millis();
}


//============
void Sensors::StopEncoders() {  
    this->counter_active = false;
}


//============
float Sensors::GetSpeedLeft() { 
    this->StopEncoders(); 
    float expected =  this->alpha * this->counter_left /
        ((float) (millis() - this->previous_time));
    return expected/7474.322;
}


//============
float Sensors::GetSpeedRight() {  
    this->StopEncoders();
    float expected =  this->alpha * this->counter_right /
        ((float) (millis() - this->previous_time));
    return expected/7474.322;
}


//============
float Sensors::GetDistance() {
    this->StopEncoders();  
    float speed = (this->GetSpeedLeft() + this->GetSpeedRight())/2.;
    return speed * ((float) (millis() - this->previous_time));
}


//============
void Sensors::IncrementLeft() { 
    if (this->counter_active) this->counter_left++; 
}


//============
void Sensors::IncrementRight() {  
    if (this->counter_active) this->counter_right++; 
}


//============
bool Sensors::IsIntersection(Actuators* actuators) { 
    this->QTRARead();
    bool is_road_left = this->IsRoadLeft();
    bool is_road_here = this->IsRoadHere();
    bool is_road_right = this->IsRoadRight();
    if (is_road_left || is_road_right || !is_road_here) {
        actuators->Stop();
        delay(1000);
        return true;
    } 
    return false;
}


//============
byte Sensors::TypeIntersection(Actuators* actuators) {  
    bool is_road_left = this->IsRoadLeft();
    bool is_road_here = this->IsRoadHere();
    bool is_road_right = this->IsRoadRight();

    actuators->OneInch();
    delay(500);

    this->QTRARead();
    bool is_road_front = this->IsRoadHere();

    actuators->OneInch();
    delay(1000);

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








