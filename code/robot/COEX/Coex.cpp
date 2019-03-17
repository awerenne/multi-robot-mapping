#include "Coex.h"


//============
Coex::Coex(const byte* pins_messenger, const byte* pins_actuators,
            const byte* pins_qta, const byte pin_sharp, const int baud_rate,
            const unsigned int* parameters_qta) {
    messenger = new Messenger(pins_messenger, baud_rate);
    actuators = new Actuators(pins_actuators);
    sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);
    anom = new Anomalies();
    
    pid_speed = new PIDController(12, 0, 0.022);
    pid_forward = new PIDController(15, 0.07, 0.065);
    pid_line = new PIDController(50, 0.025, 0.15);
    pid_responsive = new PIDController(2, 2, 0);
    pid_speed->setMin(0);
    pid_speed->setMax(255);
    pid_forward->setMin(-255);
    pid_forward->setMax(255);
    pid_line->setMin(-255);
    pid_line->setMax(255);
    pid_line->setZeta(3.1415/2.5);

    acc_normal = new Accelerator(0.1);
    acc_rotation = new Accelerator(0.1);
   
    f_msg = new FrequencyState(20);
    f_obstacle = new FrequencyState(10);
    f_speed_ctrl = new FrequencyState(20);
    f_dir_fwd_ctrl = new FrequencyState(20);
    f_dir_line_ctrl = new FrequencyState(50);
    f_rotation = new FrequencyState(100);
    f_acc = new FrequencyState(20);

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
void Coex::newLine(const float& target_speed, const bool& with_intersection) {
    pid_speed->reset();
    pid_line->reset();
    sensors->encodersReset();
    this->with_intersection = with_intersection;
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
float Coex::followLine() {
    float dist = 0;
    if (f_obstacle->isNewState() && sensors->isObstacle()) {
        stop();
        return -1;
    }
    if (f_dir_line_ctrl->isNewState()) {
        sensors->qtraRead();
        alpha = pid_line->correction(errorLine());
        if (with_intersection && isAnomaly()) {
            if (isIntersection()) return -1;
            dist += 2.1;
        }
    }
    if (f_speed_ctrl->isNewState()) {
        sensors->encodersRead();
        dist += sensors->getDistance();
        beta = pid_speed->correction(errorSpeed());
    }
    float pwm_left = beta + alpha;
    float pwm_right = beta - alpha;
    actuators->updatePWM(pwm_left, pwm_right);
    if (f_acc->isNewState()) acc_normal->accelerate(progress_speed);
    delay(delay_);
    return dist;
}


//============
float Coex::forward() {
    float dist = 0;
    if (f_obstacle->isNewState() && sensors->isObstacle()) {
        stop();
        return -1;
    }    
    if (f_dir_fwd_ctrl->isNewState()) {
        sensors->encodersRead();
        dist = sensors->getDistance();
        alpha = pid_forward->correction(errorForward()); 
    }
    if (f_speed_ctrl->isNewState()) beta = pid_speed->correction(errorSpeed());
    float pwm_left = beta + alpha;
    float pwm_right = beta - alpha;
    actuators->updatePWM(pwm_left, pwm_right);
    if (f_acc->isNewState()) acc_normal->accelerate(progress_speed);
    delay(delay_);
    return dist;
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
bool Coex::isAnomaly() {
    if (sensors->isRoadLeft() || !sensors->isRoadCenter() || sensors->isRoadRight()) {
        anom->reset();
        anom->start(0);
        return true;
    }
    return false;
}


//============
bool Coex::isIntersection() {
    newForward(6);
    float x = 0;
    while (forward() != -1) {
        if (f_dir_fwd_ctrl->isNewState()) {
            anom->new_(x);
            anom->newLeft(sensors->isRoadLeft());
            anom->newCenter(sensors->isRoadCenter());
            anom->newRight(sensors->isRoadRight());
            if (anom->isFinished()) {
                if (anom->isIntersection()) {
                    forwardAlign(x);
                    return true;
                }
                newLine(6, true);
                return false;
            }
        }
        if (f_speed_ctrl->isNewState()) {
            sensors->encodersRead();
            x += sensors->getDistance();
        }
        delay(5);
    }
}


//============
byte Coex::typeIntersection() {
    return anom->typeIntersection();
}


//============
void Coex::forwardAlign(float x) {
    while(forward() != -1) {
        if (f_speed_ctrl->isNewState()) {
            sensors->encodersRead();
            x += sensors->getDistance();
            if (x > 4) {
                stop();
                return;
            }
        }
    }
}


//============
void Coex::turnLeft(const float& speed) {
    turnAlign(speed, 0, 0);
}


//============
void Coex::turnRight(const float& speed) {
    turnAlign(speed, 1, 0);
}


//============
void Coex::uturn(const float& speed, byte type_intersection) {
    turnAlign(speed, 2, type_intersection);
}


//============
void Coex::turnAlign(const float& speed, byte instruction, byte type_intersection) {
    acc_normal->start(progress_speed, 5, 1);
    alpha = 0;
    beta = 0;
    float pwm_left = 0, pwm_right = 0;
    pid_responsive->setParameters(2,2,0);
    float xinit = 2.25, e = 0.05, L = 10, v_min = 2, a = 0, x;
    bool clockwise = true, signal_ = false, second_pass = false;

    switch (instruction) {
        case 0:
            clockwise = false;
            second_pass = false;
            break;
        case 1:
            clockwise = true;
            second_pass = false;
            break;
        case 2:
            clockwise = false;
            if (type_intersection == 0 || type_intersection == 1
                || type_intersection == 3) second_pass = true;
            else second_pass = false;
            break;
        case 3: return;
        case 4: return;
        default: return;
    }
    if (clockwise) {
        xinit *= -1;
        e *= -1;
        pid_responsive->setParameters(1,1,0);
    }
    x = xinit;
    while (true) {
        if ((clockwise && x > e) || (!clockwise && x < e)) break;
        if (f_rotation->isNewState()) {
            sensors->encodersRead();
            sensors->qtraRead();
            bool change = signal_;
            if (clockwise) signal_ = signal_ || sensors->isRoadRight();
            else signal_ = signal_ || sensors->isRoadLeft();
            if (change != signal_) {
                x = xinit;
                a = (sensors->getSpeed() - v_min)/((xinit-e)*(xinit-e));
            }
            if (signal_) {
                alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
                float v = sensors->getSpeed();
                if (v < 0.5) break;
                x = -sensors->getError();
                x = floorf((x/1250.)*100)/100;
                float gamma = pid_responsive->correction(f(x,a,e,v_min) - v);
                beta += gamma;
                pwm_left = beta + alpha;
                pwm_left = pwm_left < 0 ? 0 : pwm_left;
                pwm_left = pwm_left > 255 ? 255 : pwm_left;
                pwm_left = clockwise ? pwm_left : -pwm_left;
                pwm_right = beta - alpha;
                pwm_right = pwm_right < 0 ? 0 : pwm_right;
                pwm_right = pwm_right > 255 ? 255 : pwm_right;
                pwm_right = clockwise ? -pwm_right : pwm_right;
                actuators->updatePWM(pwm_left, pwm_right);
            }
            else {
                alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
                if (f_speed_ctrl->isNewState()) beta = pid_speed->correction(progress_speed - sensors->getSpeed());
                pwm_left = beta + alpha;
                pwm_left = clockwise ? pwm_left : -pwm_left;
                pwm_right = beta - alpha;
                pwm_right = clockwise ? -pwm_right : pwm_right;
                actuators->updatePWM(pwm_left, pwm_right);
            }
        }
        if (!signal_ && f_acc->isNewState()) acc_normal->accelerate(progress_speed);
        delay(5);
    }
    actuators->stop();
}


//============
float Coex::f(float x, float e, float a, float v_min) {
    return a*(x-e)*(x-e) + v_min;
}
















































