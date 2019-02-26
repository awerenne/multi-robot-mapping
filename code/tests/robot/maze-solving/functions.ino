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
float turn_to_speed(float target_speed) {
    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();
    float progress_speed;
    Accelerator* acc = new Accelerator(0.1);
    acc->start(progress_speed, target_speed, 1.5);
    bool clockwise = true;
    FrequencyState *f_speed_ctrl = new FrequencyState(10);
    FrequencyState *f_dir_fwd_ctrl = new FrequencyState(50);
    FrequencyState *f_acc = new FrequencyState(10);
    float alpha = 0, beta = 0, pwm_left = 0, pwm_right = 0;
    while (true) {
        if (f_dir_fwd_ctrl->isNewState()) {
            sensors->encodersRead();
            alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
            if (f_speed_ctrl->isNewState()) beta = pid_speed->correction(target_speed - sensors->getSpeed());
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


float f(float x, float e, float a) {
    return a*(x-e)*(x-e) + 2.5;
}
