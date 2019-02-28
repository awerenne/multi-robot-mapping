/*
    Simple motor manipulation.
*/
 #include <math.h>


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
        case 4:
            test_4();
            break;
        case 5:
            test_5();
            break;
        case 6:
            test_6();
            break;
        case 7:
            test_7();
            break;
        case 8:
            test_8();
            break;
        case 9:
            test_9();
            break;
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
    delay(15000);
    Sensors* sensors = coex->getSensors();
    FrequencyState *f_speed_ctrl = new FrequencyState(10);
    FrequencyState *f_dir_ctrl = new FrequencyState(50);
    coex->newLine(6, false);
    unsigned long prev_t = millis();
    float dist = 0, mse = 0;
    float i = 1;
    while (true) {
        if (coex->followLine() == 1) {
            coex->stop();
            coex->sendMsg(String(dist) + "-" + String(sqrt(mse)));
            delay(10000);
            coex->newLine(6, false);
            continue;
        }
        if (f_speed_ctrl->isNewState()) {
            float v = sensors->getSpeed();
            float delta_t = sensors->getCounterDeltaTime();
            dist += v * delta_t / 1000;
            prev_t += delta_t;
        }
        if (f_dir_ctrl->isNewState()) {
            float se = sensors->getError();
            se *= se;
            mse = (i-1)/i*mse + se/i;
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
    delay(15000);
    Sensors* sensors = coex->getSensors();
    FrequencyState *f_speed_ctrl = new FrequencyState(10);
    FrequencyState *f_dir_ctrl = new FrequencyState(50);
    coex->newForward(6);
    unsigned int my_counter_left = 0, my_counter_right = 0;
    int start_counter = 0;
    while (true) {
        if (coex->forward() == 1) {
            coex->stop();
            coex->sendMsg(String(start_counter) + "-" + String(my_counter_left) + "-" + String(my_counter_right));
            delay(15000);
            coex->newForward(6);
            my_counter_left = 0;
            my_counter_right = 0;
            continue;
        }
        if (f_dir_ctrl->isNewState()) {
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
        Used to initial tuning.
    */ 
    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
    float beta = turn_to_speed(4);
    unsigned long prev_t = millis(), current_t = millis();
    float xinit = 2.25, e = 0.15, L=10;
    float x = xinit;
    bool clockwise = true;
    float a = (sensors->getSpeed() - 2)/((xinit-e)*(xinit-e));
    PIDController* pid_responsive = new PIDController(10, 15, 0);
    FrequencyState *f_ctrl = new FrequencyState(100);
    while (x > e) {
        if (f_ctrl->isNewState()) {
            sensors->encodersRead();
            float alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
            float v = sensors->getSpeed();
            Serial.println("\n");
            Serial.println("Speed:" + String(v));

            current_t = millis();
            float delta_t = (float) (current_t-prev_t);
            float delta_theta = 2 * v * delta_t/1000/9.3;
            prev_t = current_t;

            x -= L*delta_theta;
            Serial.println("Position: " + String(x));
            float gamma = pid_responsive->correction(f(x,a,e) - v);
            Serial.println(gamma);
            Serial.println(beta);
            beta += gamma;
            Serial.println(alpha);
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




//============
void test_4() {
    /* 
        Simulate alignment with inactive motors. 
    */
    Sensors* sensors = coex->getSensors();
    float xinit = 2, e = 0.15, L=10;
    float x = xinit;
    bool clockwise = true;
    float a = 0;
    FrequencyState *f_ctrl = new FrequencyState(100);
    bool signal = false;
    while (x > e) {
        if (f_ctrl->isNewState()) {    
            sensors->qtraRead();
            bool change = signal;
            signal = signal || sensors->isRoadLeft();
            if (change != signal) a = (6 - 2)/((xinit-e)*(xinit-e));
            if (signal) x = sensors->getError()/1250;  // To cm
        }
        delay(5);
    }
    delay(5000);
}


//============
void test_5() {
    /* 
        Turn 90 alignment 
    */
    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
    float beta = turn_to_speed(4);
    unsigned long prev_t = millis(), current_t = millis();
    float xinit = 2.25, e = 0.15, L=10;
    float x = xinit;
    bool clockwise = true;
    float a = (sensors->getSpeed() - 2)/((xinit-e)*(xinit-e));
    PIDController* pid_responsive = new PIDController(10, 15, 0);
    FrequencyState *f_ctrl = new FrequencyState(100);
    while (x > e) {
        if (f_ctrl->isNewState()) {
            sensors->encodersRead();
            float alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
            float v = sensors->getSpeed();
            Serial.println("\n");
            Serial.println("Speed:" + String(v));

            current_t = millis();
            float delta_t = (float) (current_t-prev_t);
            float delta_theta = 2 * v * delta_t/1000/9.3;
            prev_t = current_t;

            x -= L*delta_theta;
            Serial.println("Position: " + String(x));
            float gamma = pid_responsive->correction(f(x,a,e) - v);
            Serial.println(gamma);
            Serial.println(beta);
            beta += gamma;
            Serial.println(alpha);
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




//============
void test_6() {
    /* 
        Turn 180 alignment
    */
    Sensors* sensors = coex->getSensors();
    Actuators* actuators = coex->getActuators();
    float beta = turn_to_speed(4);
    unsigned long prev_t = millis(), current_t = millis();
    float xinit = 2.25, e = 0.15, L=10;
    float x = xinit;
    bool clockwise = true;
    float a = (sensors->getSpeed() - 2)/((xinit-e)*(xinit-e));
    PIDController* pid_responsive = new PIDController(10, 15, 0);
    FrequencyState *f_ctrl = new FrequencyState(100);
    while (x > e) {
        if (f_ctrl->isNewState()) {
            sensors->encodersRead();
            float alpha = pid_forward->correction(sensors->getSpeedRight() - sensors->getSpeedLeft());
            float v = sensors->getSpeed();
            Serial.println("\n");
            Serial.println("Speed:" + String(v));

            current_t = millis();
            float delta_t = (float) (current_t-prev_t);
            float delta_theta = 2 * v * delta_t/1000/9.3;
            prev_t = current_t;

            x -= L*delta_theta;
            Serial.println("Position: " + String(x));
            float gamma = pid_responsive->correction(f(x,a,e) - v);
            Serial.println(gamma);
            Serial.println(beta);
            beta += gamma;
            Serial.println(alpha);
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



//============
void test_7() {
    /* 
        Line following with intersection detection. Counts anomalies, stops
        when intersection, sends percentages via bluetooth.
    */
    ;
}



//============
void test_8() {
    /* 
        Line following with intersection detection + forward until aligned for
        manoeuver. Send results via bleutooth.
    */
    ;
}


//============
void test_9() {
    /* 
        Follows line. detect at intersection. classifies intersection. determines
        direction with LHR. turns in the determined direction with alignment.
        Loops. (all of this without stopping in a fluent motion)
    */
    ;
}
