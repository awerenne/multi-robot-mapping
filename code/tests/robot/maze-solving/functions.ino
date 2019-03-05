/* 
    ..
*/


//============
void flicker_led(byte led, unsigned int n, unsigned int delay_) {
    for (int i = 0; i < n; i++) {
        digitalWrite(led, LOW);
        delay(delay_);
        digitalWrite(led, HIGH);
        delay(delay_);
    }
}


//============
float turn_to_speed(float target_speed, bool clockwise) {
    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();
    float progress_speed;
    Accelerator* acc = new Accelerator(0.2);
    acc->start(progress_speed, target_speed, 2.5);
    FrequencyState *f_speed_ctrl = new FrequencyState(10);
    FrequencyState *f_turn_ctrl = new FrequencyState(50);
    FrequencyState *f_acc = new FrequencyState(20);
    float alpha = 0, beta = 0, pwm_left = 0, pwm_right = 0;
    while (true) {
        if (f_turn_ctrl->isNewState()) {
            sensors->encodersRead();
            alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
            if (f_speed_ctrl->isNewState()) beta = pid_speed->correction(progress_speed - sensors->getSpeed());
            pwm_left = beta + alpha;
            pwm_left = clockwise ? pwm_left : -pwm_left;
            pwm_right = beta - alpha;
            pwm_right = clockwise ? -pwm_right : pwm_right;
        }
        actuators->updatePWM(pwm_left, pwm_right);
        if (f_acc->isNewState()) acc->accelerate(progress_speed);
        if (progress_speed >= target_speed && sensors->getSpeed() > target_speed) break;
        delay(5);
    }
    return beta;
}


//============
float f(float x, float e, float a, float v_min) {
    return a*(x-e)*(x-e) + v_min;
}


//============
byte lhr(byte type_intersection) {
    switch(type_intersection) {
        case 0: return 0;
        case 1: return 0;
        case 2: return 3;
        case 3: return 0;
        case 4: return 0;
        case 5: return 3;
        case 6: return 2;
        default: 4;
    }
}



//============
void turn_align(byte instruction, byte type_intersection, Sensors* sensors,
        Actuators* actuators) {
    pid_speed->reset();
    pid_forward->reset();
    PIDController* pid_responsive = new PIDController(2, 2, 0);
    sensors->encodersReset();

    float progress_speed;
    Accelerator* acc = new Accelerator(0.1);
    acc->start(progress_speed, 3, 1);
    FrequencyState *f_ctrl = new FrequencyState(100);
    FrequencyState *f_acc = new FrequencyState(20);
    FrequencyState *f_speed_ctrl = new FrequencyState(20);

    float xinit = 2.25, e = 0.05, L = 10, v_min = 2, a = 0, x;
    float alpha = 0, beta = 0, pwm_left = 0, pwm_right = 0;
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
        if (f_ctrl->isNewState()) {
            sensors->encodersRead();
            sensors->qtraRead();
            bool change = signal_;
            if (clockwise) signal_ = signal_ || sensors->isRoadRight();
            else signal_ = signal_ || sensors->isRoadLeft();
            if (change != signal_) {
                if (second_pass) {
                    second_pass = false;
                    signal_ = change;
                    delay(200);
                    continue;
                }
                x = xinit;
                a = (sensors->getSpeed() - v_min)/((xinit-e)*(xinit-e));
            }
            if (signal_) {
                alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
                float v = sensors->getSpeed();
                if (v < 0.5) break;
                x = ((float) sensors->getError())/1250.;
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
        if (!signal_ && f_acc->isNewState()) acc->accelerate(progress_speed);
        delay(5);
    }
    actuators->stop();
    delay(100);
}



























