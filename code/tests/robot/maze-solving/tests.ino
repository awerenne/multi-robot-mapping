/*
    Simple motor manipulation.
*/



//============
void test(byte test_id) {
    switch (test_id) {
        case 1:
            test_1();
            break;
        case 2:
            test_2();
            break;
        case 3:
            test_3();
            break;
    }
}



//============
void test_1() {
    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(5000);
    Sensors* sensors = coex->getSensors();
    FrequencyState *f_speed_ctrl = new FrequencyState(10);
    FrequencyState *f_dir_ctrl = new FrequencyState(50);
    coex->newLine(6, false);
    unsigned long prev_t = millis();
    float dist = 0, mean = 0;
    float i = 1;
    while (true) {
        if (coex->followLine() == 1) {
            coex->stop();
            coex->sendMsg(String(dist) + "-" + String(mean));
            break;
        }
        if (f_speed_ctrl->isNewState()) {
            float v = sensors->getSpeed();
            float delta_t = (float) (millis()-prev_t);
            dist += v * delta_t / 1000;
            prev_t += delta_t;
        }
        if (f_dir_ctrl->isNewState()) {
            float err = sensors->getError();
            err = err < 0 ? -err : err;
            mean = (i-1)/i*mean + err/i;
            i++;
        }
        delay(5);
    }
    delay(5000);
}


//============
void test_2() {
    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(5000);
    Sensors* sensors = coex->getSensors();
    FrequencyState *f_speed_ctrl = new FrequencyState(10);
    FrequencyState *f_dir_ctrl = new FrequencyState(50);
    coex->newForward(6);
    unsigned long prev_t = millis();
    unsigned int my_counter_left = 0, my_counter_right = 0;
    while (true) {
        if (coex->forward() == 1) {
            coex->stop();
            coex->sendMsg(String(my_counter_left) + "-" + String(my_counter_right));
            break;
        }
        if (f_dir_ctrl->isNewState()) {
            if (sensors->isRoadLeft()) my_counter_left++;
            if (sensors->isRoadRight()) my_counter_right;
        }
        delay(5);
    }
    delay(5000);
}


//============
void test_3() {
    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
    float beta = turn_to_speed(6);
    unsigned long prev_t = millis(), current_t = millis();
    float xinit = 2, e = 0.2, L=10;
    float x = xinit;
    bool clockwise = true;
    float a = (sensors->getSpeed() - 2.5)/((xinit-e)*(xinit-e));
    PIDController* pid_responsive = new PIDController(0.4, 0.00, 0.0);
    FrequencyState *f_ctrl = new FrequencyState(50);
    while (x > e) {
        if (f_ctrl->isNewState()) {
            sensors->encodersRead();
            float alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
            float v = sensors->getSpeed();
            Serial.println(String(v) + " - " + String(x));

            current_t = millis();
            float delta_t = (float) (current_t-prev_t);
            Serial.println(String(delta_t));
            float delta_theta = 2 * v * delta_t/1000/9.3;
            prev_t = current_t;

            x -= L*delta_theta;
            Serial.println(String(x));
            float gamma = pid_responsive->correction(f(x,a,e) - v);
            Serial.println(String(gamma));
            beta += gamma;
            Serial.println(String(beta));
            float pwm_left = beta + alpha;
            pwm_left = clockwise ? pwm_left : -pwm_left;
            float pwm_right = beta - alpha;
            pwm_right = clockwise ? -pwm_right : pwm_right;
            actuators->updatePWM(pwm_left, pwm_right);
        }
        delay(5);
    }
    actuators->stop();
    delay(5000);
}
