#include "Sensors.h"


//============
Sensors::Sensors(const byte* pins_qta, const unsigned int* parameters,
        const byte& pin_sharp) {
    qtra = new QTRSensorsAnalog(pins_qta, parameters[1], parameters[2]);
    num_sensors = parameters[1];
    sensor_values = (unsigned int*) malloc(num_sensors * sizeof(unsigned int));
    this->pin_sharp = pin_sharp;
    pinMode(this->pin_sharp, INPUT);
    encoder_left = new EncoderState();
    encoder_right = new EncoderState();
}


//============
void Sensors::manualCalibration() {
    for (int i = 0; i < 400; i++) qtra->calibrate();  
}


//============
void Sensors::automaticCalibration(Actuators* actuators) {
    int clockwise = -1;
    int pwm_left = 85, pwm_right = 85;
    bool first_loop = true;
    for (int i = 0; i < 200;) {
        actuators->updatePWM(clockwise * pwm_left, -clockwise * pwm_right);
        if (first_loop) {
            for (int j = 0; j < 10; j++) qtra->calibrate();  
            i += 10;
        }
        else {
            for (int j = 0; j < 20; j++) qtra->calibrate();  
            i += 20;
        }
        actuators->stop();
        clockwise *= -1;
        first_loop = false;
        delay(200);
    }
    actuators->updatePWM(clockwise * pwm_left, -clockwise * pwm_right);
    for (int j = 0; j < 10; j++) qtra->calibrate(); 
    actuators->stop();
}



//============
void Sensors::oneStepCalibration() {
    qtra->calibrate(); 
}


//============
bool Sensors::ambientIsDark() {
    qtra->read(sensor_values, QTR_EMITTERS_OFF); 
    int min_value = 1000;
    for (int i = 0; i < num_sensors; i++) 
        if (sensor_values[i] < min_value) min_value = sensor_values[i];
    if (min_value >= 400) return true;
    return false;
}


//============
void Sensors::setCalibration(unsigned int* calib_min, unsigned int* calib_max) {
    qtra->calibratedMinimumOn = calib_min;
    qtra->calibratedMaximumOn = calib_max;
}      


// //============
// void Sensors::automaticCalibration() {
//     unsigned int calib_min_dark[6] = {100,100,100,100,100,100};  
//     unsigned int calib_max_dark[6] = {750,750,750,750,750,750}; 
//     unsigned int calib_min_light[6] = {100,100,100,100,100,100};  
//     unsigned int calib_max_light[6] = {600,600,600,600,600,600};  
//     if (ambientIsDark()) setCalibration(calib_min_dark, calib_max_dark);
//     else setCalibration(calib_min_light, calib_max_light);
// }


//============
void Sensors::qtraRead() {
    position = qtra->readLine(sensor_values);
}



//============
bool Sensors::isRoadLeft() {  
    return sensor_values[0] > 700;
    // return sensor_values[0] > 600;
}


//============
bool Sensors::isRoadRight() {  
    return sensor_values[num_sensors-1] > 650;
    // return sensor_values[num_sensors-1] > 600;
}


//============
bool Sensors::isRoadCenter() {  
    float sum = 0;
    for (int i = 1; i < num_sensors-1; i++) {
        sum = sensor_values[i];
        if (sensor_values[i] > 600) return true;
        // if (sensor_values[i] > 750 & getError() > -1850 & getError() < 1850) return true;
    }
    // if (sum > 1200)
    //     return true;
    return false;
}


//============
bool Sensors::isRoadFront() {  
    return (sensor_values[2] + sensor_values[3])/2 > 600;
}


//============
bool Sensors::isAligned() {  
    return false;
}


//============
int Sensors::getError() {
    /*
        Return x:
            x < 0 if line is left of center
            x > 0 if line is right of center
    */ 
    int center = (num_sensors-1)*1000/2;
    return (position-center);
}


//============
bool Sensors::isObstacle() {
    if (sharpVoltage() < 0.4) return true;
    return false;
}


//============
float Sensors::sharpVoltage() {
    int value = analogRead(pin_sharp);  
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
    return angularVelocity(counts, time) * radius_wheel;
}



//============
void Sensors::encodersReset() {
    encoder_left->reset();
    encoder_right->reset();
}


//============
void Sensors::incrementLeft() {
    encoder_left->incrementCounter();
}


//============
void Sensors::incrementRight() {
    encoder_right->incrementCounter();
}


//============
void Sensors::encodersRead() {
    encoder_left->newTimeStep();
    encoder_right->newTimeStep();
}


//============
float Sensors::getSpeed() { 
    float v_left = getSpeedLeft();
    float v_right = getSpeedRight();
    return (v_left + v_right)/2; 
}


//============
float Sensors::getSpeedLeft() { 
    float delta_counter = encoder_left->deltaCounter();
    float delta_time = encoder_left->deltaTime();
    return linearVelocity(delta_counter, delta_time);
}


//============
float Sensors::getSpeedRight() {  
    float delta_counter = encoder_right->deltaCounter();
    float delta_time = encoder_right->deltaTime();
    return linearVelocity(delta_counter, delta_time);
}


//============
float Sensors::getCounterDeltaTime() {
    return (encoder_left->deltaTime() + encoder_right->deltaTime())/2; 
}


//============
float Sensors::getDistance() {
    float v = getSpeed();
    float delta_t = getCounterDeltaTime();
    return 2 * v * delta_t / 1000; 
}



















