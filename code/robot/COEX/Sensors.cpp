#include "Sensors.h"


//============
Sensors::Sensors(const byte* pins_qta, const unsigned int* parameters,
        const byte& pin_sharp) {
    this->qtra = new QTRSensorsAnalog(pins_qta, parameters[1], parameters[2]);
    this->num_sensors = parameters[1];
    this->sensor_values = (unsigned int*) malloc(this->num_sensors * sizeof(unsigned int));
    this->pin_sharp = pin_sharp;
    pinMode(this->pin_sharp, INPUT);
}


//============
void Sensors::manualCalibration() {
    for (int i = 0; i < 400; i++) this->qtra->calibrate();  
}


//============
bool Sensors::ambientIsDark() {
    qtra->read(sensor_values, QTR_EMITTERS_OFF); 
    for (int i = 0; i < this->num_sensors; i++) 
        if (this->sensor_values[i] > 300) return true;
    return false;
}


//============
void Sensors::setCalibration(unsigned int* calib_min, unsigned int* calib_max) {
    this->qtra->calibratedMinimumOn = calib_min;
    this->qtra->calibratedMaximumOn = calib_max;
}      


//============
void Sensors::automaticCalibration() {
    unsigned int calib_min_dark[6] = {270,124,126,112,117,260};  
    unsigned int calib_max_dark[6] = {720,615,620,590,595,710}; 
    unsigned int calib_min_light[6] = {120,110,110,110,110,175};  
    unsigned int calib_max_light[6] = {720,615,620,590,595,710};  
    if (this->ambientIsDark())
        this->setCalibration(calib_min_dark, calib_max_dark);
    else
        this->setCalibration(calib_min_light, calib_max_light);
}


//============
void Sensors::qtraRead() {
    this->position = this->qtra->readLine(this->sensor_values);
}



//============
bool Sensors::isRoadLeft() {  
    return (this->sensor_values[this->num_sensors-1] > 600);
}


//============
bool Sensors::isRoadRight() {  
    return (this->sensor_values[0] > 600);
}


//============
bool Sensors::isRoadCenter() {  
    float mean = 0;
    for (int i = 1; i < 5; i++) mean += this->sensor_values[i];
    mean /= 4; 
    return (mean > 600);
}


//============
bool Sensors::isAligned() {  
    return false;
}


//============
int Sensors::getError() {
    /*
        Return x:
            x > 0 if line is left of center
            x < 0 if line is right of center
    */ 
    int center = (this->num_sensors-1)*1000/2;
    return (position-center);
}


//============
bool Sensors::isObstacle() {
    if (this->sharpVoltage() < 0.4) return true;
    return false;
}


//============
float Sensors::sharpVoltage() {
    int value = analogRead(this->pin_sharp);  
    return ((float) value)/1024.;       
}


//============
float Sensors::angularVelocity(const float& counts, const float& time) {
    /* 
        Radians per second
    */
    float gearbox_ration = 150.58, counts_per_revolution = 6;  
    float n_revolutions = counts / (gearbox_ration * counts_per_revolution);
    float frequency = n_revolutions / time * 1000;  // number of revol. per sec
    return 2. * 3.1415 * frequency;
}


//============
float Sensors::linearVelocity(const float& counts, const float& time) {
    /* 
        Centimeters per second
    */
    float radius_wheel = 0.8;  // cm
    return this->angularVelocity(counts, time) * radius_wheel;
}



//============
void Sensors::encodersReset() {
    this->encoder_left->resetCounter();
    this->encoder_right->resetCounter();
}


//============
void Sensors::incrementLeft() {
    this->encoder_left->incrementCounter();
}


//============
void Sensors::incrementRight() {
    this->encoder_right->incrementCounter();
}


//============
void Sensors::encodersRead() {
    this->encoder_left->newTimeStep();
    this->encoder_right->newTimeStep();
}


//============
float Sensors::getSpeed() { 
    float v_left = this->getSpeedLeft();
    float v_right = this->getSpeedRight();
    return (v_left + v_right)/2; 
}


//============
float Sensors::getSpeedLeft() { 
    float delta_counter = this->encoder_left->deltaCounter();
    float delta_time = this->encoder_left->deltaTime();
    return this->linearVelocity(delta_counter, delta_time);
}


//============
float Sensors::getSpeedRight() {  
    float delta_counter = this->encoder_right->deltaCounter();
    float delta_time = this->encoder_right->deltaTime();
    return this->linearVelocity(delta_counter, delta_time);
}


//============
float Sensors::getDistance() {
    return 0; 
}


//============
EncoderState::EncoderState() {
    this->resetCounter();
}


//============
void EncoderState::resetCounter() {
    this->counter = 0;
    this->prev_counter = 0;
    this->prev_time = millis();
}


//============
void EncoderState::newTimeStep() {
    this->prev_counter = this->current_counter;
    this->current_counter = this->counter;
    this->prev_time = this->current_time;
    this->current_time = millis();
}


//============
void EncoderState::incrementCounter() {
    this->counter++;
}


//============
float EncoderState::deltaTime() {
    return (float) (this->current_time - this->prev_time);
}


//============
float EncoderState::deltaCounter() {
    return (float) (this->current_counter - this->prev_counter);
}















