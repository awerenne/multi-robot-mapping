#include "Coex.h"


//============
Coex::Coex(const byte* pins_messenger, const byte* pins_actuators,
            const byte* pins_qta, const byte pin_sharp, const int baud_rate,
            const unsigned int* parameters_qta) {
    messenger = new Messenger(pins_messenger, baud_rate);
    actuators = new Actuators(pins_actuators);
    sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);
    
    pid_speed = new PIDController(12, 0, 0.022);
    pid_forward = new PIDController(15, 0.07, 0.065);
    pid_line = new PIDController(50, 0.025, 0.15);
    pid_speed->setMin(0);
    pid_speed->setMax(255);
    pid_forward->setMin(-255);
    pid_forward->setMax(255);
    pid_line->setMin(-255);
    pid_line->setMax(255);
    pid_line->setZeta(3.1415/2);

    acc_normal = new Accelerator(0.1);
    acc_rotation = new Accelerator(0.1);
   
    f_msg = new FrequencyState(20);
    f_obstacle = new FrequencyState(10);
    f_speed_ctrl = new FrequencyState(10);
    f_dir_fwd_ctrl = new FrequencyState(50);
    f_dir_line_ctrl = new FrequencyState(50);
    f_acc = new FrequencyState(20);

    with_distance = false;
    delay_ = 5;
    alpha = 0;
    beta = 0;
}


//============
void Coex::stop() {
    acc_normal->stop(progress_speed);
    acc_rotation->stop(progress_speed);
    actuators->stop();
}


//============
bool Coex::availableMsg() {
    if (f_msg->isNewState()) return messenger->receiveMessage();
    return false;
}


//============
void Coex::readMsg() {
    messenger->parseMessage();
}


//============
int Coex::getMsgInstruction() {
    return messenger->getMessage()[0];
}


//============
void Coex::newLine(const float& target_speed, const bool& with_distance) {
    pid_speed->reset();
    pid_line->reset();
    this->with_distance = with_distance;
    sensors->encodersReset();
    setTargetSpeed(target_speed);
    alpha = 0;
    beta = 0;
}


//============
void Coex::newForward(const float& target_speed) {
    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();
    setTargetSpeed(target_speed);
    alpha = 0;
    beta = 0;
}


//============
void Coex::setTargetSpeed(const float& target_speed) {
    this->target_speed = target_speed;
    acc_normal->start(progress_speed, target_speed, 1.5);
}


//============
float Coex::errorLine() {
    float err = sensors->getError();
    return floorf((err/1250.)*100)/100; 
}


//============
float Coex::errorSpeed() {
    return progress_speed - sensors->getSpeed();
}


//============
float Coex::errorForward() {
    return sensors->getSpeedRight() - sensors->getSpeedLeft();
}


//============
byte Coex::followLine() {
    if (f_obstacle->isNewState() && sensors->isObstacle()) {
        stop();
        return 1;
    }
    if (f_dir_line_ctrl->isNewState()) {
        sensors->qtraRead();
        alpha = pid_line->correction(errorLine());
        if (f_speed_ctrl->isNewState()) {
            sensors->encodersRead();
            beta = pid_speed->correction(errorSpeed());
        }
        float pwm_left = beta + alpha;
        float pwm_right = beta - alpha;
        actuators->updatePWM(pwm_left, pwm_right);
    }
    if (f_acc->isNewState()) acc_normal->accelerate(progress_speed);
    delay(delay_);
    return 0;
}


//============
byte Coex::forward() {
    if (f_obstacle->isNewState() && sensors->isObstacle()) {
        stop();
        return 1;
    }    
    if (f_dir_fwd_ctrl->isNewState()) {
        sensors->encodersRead();
        alpha = pid_forward->correction(errorForward()); 
        if (f_speed_ctrl->isNewState()) 
            beta = pid_speed->correction(errorSpeed());
        float pwm_left = beta + alpha;
        float pwm_right = beta - alpha;
        actuators->updatePWM(pwm_left, pwm_right);
    }
    if (f_acc->isNewState()) acc_normal->accelerate(progress_speed);
    delay(delay_);
    return 0;
}


//============
byte Coex::turn90(const bool& clockwise, const float& target_speed) {    
    acc_rotation->stop(progress_speed);
    acc_rotation->start(progress_speed, target_speed, 1);
    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();
    return turnTheta(3.1415/4, clockwise);
}


//============
byte Coex::turn180(const bool& clockwise, const float& target_speed) {
    acc_rotation->stop(progress_speed);
    acc_rotation->start(progress_speed, target_speed, 1);
    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();
    return turnTheta(3.1415/2, clockwise);
}


//============
byte Coex::turnTheta(const float& Theta, const bool& clockwise) {
    float alpha = 0, beta = 0, pwm_left = 0, pwm_right = 0, theta = 0;
    unsigned long prev_t = millis(), current_t = millis();
    while (theta < Theta) {
        if (f_obstacle->isNewState() && sensors->isObstacle()) return 1;
        if (f_dir_fwd_ctrl->isNewState()) {
            sensors->encodersRead();
            alpha = pid_forward->correction(errorForward());
            if (f_speed_ctrl->isNewState()) beta = pid_speed->correction(errorSpeed());
            pwm_left = beta + alpha;
            pwm_left = clockwise ? pwm_left : -pwm_left;
            pwm_right = beta - alpha;
            pwm_right = clockwise ? -pwm_right : pwm_right;
            current_t = millis();
            float delta_t = (float) (current_t-prev_t);
            theta += 2 * sensors->getSpeed() * delta_t/1000/9.3;
            prev_t = current_t;
        }
        
        actuators->updatePWM(pwm_left, pwm_right);
        if (f_acc->isNewState()) acc_rotation->accelerate(progress_speed);
        delay(delay_);
    }       
    acc_rotation->stop(progress_speed);
    actuators->stop();
    delay(delay_);
    return 0;
}


//============
void Coex::oneInch(const float& speed) {
    ;
}


//============
void Coex::turnLeft(const float& speed) {
    ;
}


//============
void Coex::turnRight(const float& speed) {
    ;
}


//============
void Coex::uturn(const float& speed) {
    ;
}


//============
bool Coex::isIntersection() {
    ;
    // this->QTRARead();
    // bool is_road_left = this->IsRoadLeft();
    // bool is_road_here = this->IsRoadHere();
    // bool is_road_right = this->IsRoadRight();
    // if (is_road_left || is_road_right || !is_road_here) {
    //     actuators->Stop();
    //     delay(1000);
    //     return true;
    // } 
    // return false;
}


//============
byte Coex::typeIntersection() {
    ;
    // bool is_road_left = this->IsRoadLeft();
    // bool is_road_here = this->IsRoadHere();
    // bool is_road_right = this->IsRoadRight();

    // actuators->OneInch();
    // delay(500);

    // this->QTRARead();
    // bool is_road_front = this->IsRoadHere();

    // actuators->OneInch();
    // delay(1000);

    // if (is_road_left && is_road_front && is_road_right)
    //     return 0;
    // else if (is_road_left && is_road_front && !is_road_right)
    //     return 1;
    // else if (!is_road_left && is_road_front && is_road_right)
    //     return 2;
    // else if (is_road_left && !is_road_front && is_road_right)
    //     return 3;
    // else if (is_road_left && !is_road_front && !is_road_right)
    //     return 4;
    // else if (!is_road_left && !is_road_front && is_road_right)
    //     return 5;
    // return 6;
}



















