/*
    Simple motor manipulation.
*/
 #include <math.h>


//============
void test(byte test_id) {
    switch (test_id) {
        case 1: test_1(); break;
        case 2: test_2(); break;
        case 3: test_3(); break;
        case 4: test_4(); break;
        case 5: test_5(); break;
        case 6: test_6(); break;
        case 7: test_7(); break;
        case 8: test_8(); break;
        case 9: test_9(); break;
        case 10: test_10(); break;
    }
}



//============
void test_1() {
    /* 
        Distance test.
        observation: distances are two time too low, speed seems underestimated
        by a factor of 2. Check encoder test for revolutions count, if correct
        then error in equations -> multiply by 2 before redoing tests and
        important performance tuning. However very bizarre because theta seems
        to be correct. (saturday) 
    */

    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(10000);
    Sensors* sensors = coex->getSensors();
    coex->newLine(6, false);
    FrequencyState *f_speed_ctrl = new FrequencyState(20);
    FrequencyState *f_dir_ctrl = new FrequencyState(50);
    
    unsigned long prev_t = millis();
    float i = 1, dist = 0, mse = 0;
    while (true) {
        if (coex->followLine() == 1) {
            coex->sendMsg(String(dist) + "-" + String(sqrt(mse)));
            break;
        }
        if (f_speed_ctrl->isNewState()) {
            float v = sensors->getSpeed();
            float delta_t = sensors->getCounterDeltaTime();
            dist += v * delta_t / 1000;
            prev_t += delta_t;
        }
        if (f_dir_ctrl->isNewState()) {
            float se = sensors->getError();
            se *= se;  // Squared error
            mse = (i-1)/i*mse + se/i;  // Rolling average mean
            i++;
        }
        delay(5);
    }
    delay(5000);
}


//============
void test_2() {
    /*
        Intersection test: forward on short distance, count anomalies on both sides,
        stop at obstacle, send anomalies counting.
    */

    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(10000);
    Sensors* sensors = coex->getSensors();
    coex->newForward(5);
    FrequencyState *f_speed_ctrl = new FrequencyState(10);
    FrequencyState *f_sensors = new FrequencyState(100);

    int my_counter_left = 0, my_counter_right = 0, start_counter = 0, counter = 0;
    while (true) {
        if (coex->forward() == 1) {
            coex->sendMsg(String(counter) + "-" + String(start_counter) + "-"\
                + String(my_counter_left) + "-" + String(my_counter_right));
            break;
        } 
        if (f_sensors->isNewState()) {
            counter++;
            sensors->qtraRead();
            if (sensors->isRoadLeft()) my_counter_left++;
            if (sensors->isRoadRight()) my_counter_right++;
            if (my_counter_left > 0 || my_counter_right > 0) start_counter++;
        }
        delay(5);
    }
    delay(5000);
}


//============
void test_3() {
    /* 
        Static test of alignment with simulation of sensors reading.
        Used to initial tuning. Do it in both directions!!!
    */ 

    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
    PIDController* pid_responsive = new PIDController(2, 2, 0);
    FrequencyState *f_ctrl = new FrequencyState(100);

    bool clockwise = false;
    float beta = turn_to_speed(3, clockwise);

    unsigned long prev_t = millis(), current_t = millis();
    float xinit = 2.25, e = 0.15, L = 10, v_min = 1.2, x;
    if (clockwise) {
        xinit *= -1;
        e *= -1;
        pid_responsive->setParameters(1,1,0);
    }
    float a = (sensors->getSpeed() - v_min)/((xinit-e)*(xinit-e));
    x = xinit;
    while (true) {
        if ((clockwise && x > e) || (!clockwise && x < e)) break;
        if (f_ctrl->isNewState()) {
            sensors->encodersRead();
            float alpha = pid_forward->correction(sensors->getSpeedRight() -\
                    sensors->getSpeedLeft());
            float v = sensors->getSpeed();
            if (v < v_min) break;

            current_t = millis();
            float delta_t = (float) (current_t-prev_t);
            float delta_theta = 2 * v * delta_t/(1000*9.3);
            if (clockwise) x += L*delta_theta;
            else x -= L*delta_theta;
            prev_t = current_t;
            
            float gamma = pid_responsive->correction(f(x,a,e,v_min) - v);
            beta += gamma;
            float pwm_left = beta + alpha;
            pwm_left = pwm_left < 0 ? 0 : pwm_left;
            pwm_left = pwm_left > 255 ? 255 : pwm_left;
            pwm_left = clockwise ? pwm_left : -pwm_left;
            float pwm_right = beta - alpha;
            pwm_right = pwm_right < 0 ? 0 : pwm_right;
            pwm_right = pwm_right > 255 ? 255 : pwm_right;
            pwm_right = clockwise ? -pwm_right : pwm_right;
            actuators->updatePWM(pwm_left, pwm_right);

            Serial.println("\n");
            Serial.println("Speed:" + String(v));
            Serial.println("Estimated position: " + String(x));
            Serial.println("Gamma: " + String(gamma));
            Serial.println("New beta: " + String(beta));
            Serial.println("Alpha: " + String(alpha));
        }
        delay(5);
    }
    actuators->stop();
    delay(5000);
}




//============
void test_4() {
    /* 
        Simulate alignment with inactive motors. 
    */

    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(10000);
    Sensors* sensors = coex->getSensors();
    FrequencyState *f_ctrl = new FrequencyState(100);

    float xinit = 2.25, e = 0.25, L = 10, v_min = 1.5, a = 0, x;
    bool clockwise = false, signal_ = false;
    if (clockwise) {
        xinit *= -1;
        e *= -1;
    }
    x = xinit;
    while (true) {
        if ((clockwise && x > e) || (!clockwise && x < e)) break;
        if (f_ctrl->isNewState()) {    
            sensors->qtraRead();
            bool change = signal_;
            if (clockwise) signal_ = signal_ || sensors->isRoadRight();
            else signal_ = signal_ || sensors->isRoadLeft();
            if (change != signal_) {
                x = xinit;
                a = (6 - v_min)/((xinit-e)*(xinit-e));
                Serial.println("Trigger");
            }
            if (signal_) {
                x = ((float) sensors->getError())/1250.;  // To cm
                Serial.println(x);
            }
        }
        delay(5);
    }
    delay(5000);
}


//============
void test_5() {
    /* 
        Turn in one direction and straight stop when signal. Be sure
        it works in both directions in light and dark environments. And that it
        stops before being aligned (determine max turning speed and acceleration).
        Otherwise it does not make sense to play with alignment. 
    */

    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(4000);
    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
    pid_speed->reset();
    pid_forward->reset();
    PIDController* pid_responsive = new PIDController(10, 15, 0);
    sensors->encodersReset();
    float progress_speed;
    Accelerator* acc = new Accelerator(0.2);
    acc->start(progress_speed, 3, 1);
    FrequencyState *f_ctrl = new FrequencyState(100);
    FrequencyState *f_acc = new FrequencyState(20);
    FrequencyState *f_speed_ctrl = new FrequencyState(20);

    float xinit = 2.25, e = 0.15, L = 10, v_min = 1.25, a = 0, x;
    float alpha = 0, beta = 0, pwm_left = 0, pwm_right = 0;
    bool clockwise = true, signal_ = false;
    if (clockwise) {
        xinit *= -1;
        e *= -1;
        pid_responsive->setParameters(0.8,0.8,0);
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
                x = xinit;
                a = (sensors->getSpeed() - v_min)/((xinit-e)*(xinit-e));
                Serial.println("Trigger");
            }
            if (signal_) {
                actuators->stop();
                break;
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
        if (f_acc->isNewState()) acc->accelerate(progress_speed);
        delay(5);
    }
    delay(5000);
}


//============
void test_6() {
    /* 
        Turn in one direction and align with pid correction. Test in both 
        directions.
    */

    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(4000);
    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
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
    bool clockwise = true, signal_ = false;
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
                x = xinit;
                a = (sensors->getSpeed() - v_min)/((xinit-e)*(xinit-e));
                Serial.println("Trigger");
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
    delay(5000);
}



//============
void test_7() {
    /* 
        Turn with alignment but pass first line (uturn if road left and right)
    */

    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(4000);
    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
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
    bool clockwise = true, signal_ = false, second_pass = true;
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
//                    unsigned long prev_t = millis(), current_t = millis();
//                    float theta = 0;
//                    while (theta < 3.1415/10) {
//                        if (f_ctrl->isNewState()) {
//                            sensors->encodersRead();
//                            alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
//                            if (f_speed_ctrl->isNewState()) beta = pid_speed->correction(progress_speed - sensors->getSpeed());
//                            pwm_left = beta + alpha;
//                            pwm_left = clockwise ? pwm_left : -pwm_left;
//                            pwm_right = beta - alpha;
//                            pwm_right = clockwise ? -pwm_right : pwm_right;
//                            actuators->updatePWM(pwm_left, pwm_right);
//                            current_t = millis();
//
//                            float delta_t = (float) (current_t-prev_t);
//                            theta += 2 * sensors->getSpeed() * delta_t/1000/9.3;
//                            prev_t = current_t;
//                        }
//                        if (f_acc->isNewState()) acc->accelerate(progress_speed);
//                        delay(5);
//                    }
                }
                x = xinit;
                a = (sensors->getSpeed() - v_min)/((xinit-e)*(xinit-e));
                Serial.println("Trigger");
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
    delay(5000);
}



//============
void test_8() {
    /* 
        Line following with intersection detection. Counts anomalies, stops
        when intersection, sends percentages via bluetooth.
    */
    
    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(10000);
    Sensors* sensors = coex->getSensors();
    coex->newLine(6, false);
    FrequencyState *f_speed_ctrl = new FrequencyState(20);
    FrequencyState *f_sensors = new FrequencyState(100);
    Anomalies* anom = new Anomalies();
   
    float dist = 0;
    unsigned long prev_t = millis();
    bool detection = false;
    int my_counter_left = 0, my_counter_right = 0, start_counter = 0, counter = 0;
    while (true) {
        byte ret;
        if (detection) ret = coex->forward();
        else ret = coex->followLine();
        if (ret == 1) {
            coex->stop();
            break;
        } 
        if (f_sensors->isNewState()) {
            sensors->qtraRead();
            if (!detection && (sensors->isRoadLeft() || 
                                    sensors->isRoadRight())) {
                anom->start(dist);
                detection = true;
                coex->newForward(6);
            }
            if (detection) {
                anom->newLeft(sensors->isRoadLeft(), dist);
                anom->newCenter(sensors->isRoadCenter(), dist);
                anom->newRight(sensors->isRoadRight(), dist);
                if (anom->isFinished()) {
                    if (anom->isIntersection()) {
                        coex->stop();
                        coex->sendMsg(anom->getSummary());
                        break;
                    }
                    anom->reset();
                    detection = false;
                    coex->newLine(6, false);
                }
            }
        }
        if (f_speed_ctrl->isNewState()) {
            float v = sensors->getSpeed();
            float delta_t = sensors->getCounterDeltaTime();
            dist += v * delta_t / 1000;
            prev_t += delta_t;
        }
        delay(5);
    }
    delay(5000);
}



//============
void test_9() {
    /* 
        Line following with intersection detection + forward until aligned for
        manoeuver. Send results via bleutooth.
    */
    ;
}


//============
void test_10() {
    /* 
        Follows line. detect at intersection. classifies intersection. determines
        direction with LHR. turns in the determined direction with alignment.
        Loops. (all of this without stopping in a fluent motion)
    */
    ;
}
