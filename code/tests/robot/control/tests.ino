/*
    ...
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
        case 10:
            test_10();
            break;
        case 11:
            test_11();
            break;
        case 12:
            test_12();
            break;
    }
}



//============
void test_1() {
    /*
        Test bidirectional communication.
    */

    int delay_ = 5;
    FrequencyState* freq_msg = new FrequencyState(10);
    FrequencyState* freq_obstacle = new FrequencyState(0.5);


    while (true) {
        if (freq_msg->isNewState())
            receive_msg_pid();
            if (instruction == 0)
                break;
        if (freq_obstacle->isNewState() && sensors->isObstacle()) break;
        Serial.println("Kp of speed pid: " + String(pid_speed->getKp()));
        delay(delay_);   
    }
    actuators->stop();
}


//============
void test_2() {
    /*
        Test random data to visualize on interactive subplots.
    */

    int delay_ = 50;
    while (true) {
        Serial.println("5/5.5/6/4");
        delay(delay_);
        if (sensors->isObstacle()) break;
    }
}


//============
void test_3() {
    /*
        Tune PID speed motor on left motor. Target speed varies between 1cm/s 
        and 2 cm/s as a wave function. (robot surélevé)

        Best tuning:
        Kp left used battery = 20 
        Ki left used battery = 0,022
        f = 5
        Kp left charged battery =  
        Ki left charged battery = 
        Kp right = 
        Ki right = 
    */

    pid_speed->reset();
    sensors->encodersReset();

    int delay_ = 5;
    FrequencyState* freq_receiver = new FrequencyState(10);
    FrequencyState* freq_sender = new FrequencyState(20);
    FrequencyState* freq_obstacle = new FrequencyState(5);
    FrequencyState* freq_wave = new FrequencyState(0.25);
    FrequencyState* freq_speed_control = new FrequencyState(5);

    int pwm = 0;
    float target_speed = 6, beta = 0, measured_speed = 0;
    
    actuators->updatePWM(pwm, 0);
    for (int seq_nb = 0; true; delay(delay_)) {
        if (freq_receiver->isNewState()) receive_msg_pid();   
        if (freq_obstacle->isNewState() && sensors->isObstacle()) break;
        if (freq_speed_control->isNewState()) {
            sensors->encodersRead();
            beta = speed_control_left(target_speed);
            pwm = beta;
        }
        if (freq_wave->isNewState()) {
            if (target_speed == 6) target_speed = 8;
            else target_speed = 6;
        }
        if (freq_sender->isNewState()) {
            measured_speed = sensors->getSpeedLeft();
            Serial.println(String(seq_nb) + "/" + String(target_speed) + "/" + String(measured_speed));
            seq_nb++;
        }
        actuators->updatePWM(pwm, 0);
    }
    actuators->stop();
}


//============
void test_4() {
    /*
        Tune PID speed control and forward control together. 
        Target speed varies between 1cm/s and 2 cm/s as a wave function. 
        (robot surélevé)

        Best tuning:
        Kp_speed = 20
        Ki_speed = 0.022
        f_speed = 5
        Kp_forward = 15
        Kd_forward = 0.07
        Ki_forward = 0.065
        f_forward = 25
    */

    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();

    int delay_ = 5;
    FrequencyState* freq_receiver = new FrequencyState(10);
    FrequencyState* freq_sender = new FrequencyState(20);
    FrequencyState* freq_obstacle = new FrequencyState(5);
    FrequencyState* freq_wave = new FrequencyState(0.01);
    FrequencyState* freq_speed_control = new FrequencyState(5);
    FrequencyState* freq_direction_control = new FrequencyState(25);

    float alpha = 0, beta = 0, target_speed = 6;
    int pwm_left = 0, pwm_right = 0;

    actuators->updatePWM(pwm_left, pwm_right);
    for (int seq_nb = 0; true; delay(delay_)) {
        if (freq_receiver->isNewState()) receive_msg_pid();
        if (freq_obstacle->isNewState() && sensors->isObstacle()) break;
        if (freq_direction_control->isNewState()) {
            sensors->encodersRead();
            alpha = forward_control();
            if (freq_speed_control->isNewState()) 
                beta = speed_control(target_speed);
        }

        pwm_left = beta + alpha;
        pwm_right = beta - alpha;
        actuators->updatePWM(pwm_left, pwm_right);

        if (freq_wave->isNewState()) {
            if (target_speed == 6) target_speed = 6;
            else target_speed = 6;
        }

        if (freq_sender->isNewState()) {
            Serial.println(String(seq_nb) + "/" + String(target_speed) + "/" +\
                String(sensors->getSpeed()) + "/" + String(sensors->getSpeedLeft())\
                + "/" + String(sensors->getSpeedRight()));
            seq_nb++;
        }
    }
    actuators->stop();
}



//============
void test_5() {
    /*
        Forward (straight line) at constant speed and stops at obstacle.
    */

    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();

    pid_speed->setParameters(20, 0, 0.022);
    pid_forward->setParameters(15, 0.07, 0.065);
    const float target_speed = 7.5;

    int delay_ = 5;
    FrequencyState* freq_obstacle = new FrequencyState(10);
    FrequencyState* freq_speed_control = new FrequencyState(5);
    FrequencyState* freq_direction_control = new FrequencyState(25);

    float alpha = 0, beta = 0;
    int pwm_left = 0, pwm_right = 0;

    actuators->updatePWM(pwm_left, pwm_right);
    for (int seq_nb = 0; true; delay(delay_)) {
        if (freq_obstacle->isNewState() && sensors->isObstacle()) break;
        if (freq_direction_control->isNewState()) {
            sensors->encodersRead();
            alpha = forward_control();
            if (freq_speed_control->isNewState()) 
                beta = speed_control(target_speed);
        }
        pwm_left = beta + alpha;
        pwm_right = beta - alpha;
        actuators->updatePWM(pwm_left, pwm_right);
    }
    actuators->stop();
}



//============
void test_6() {
    /*
        Follows a complex curved line at constant speed and stops at obstacle.
    */

    pid_speed->reset();
    pid_line->reset();
    sensors->encodersReset();

    pid_speed->setParameters(40,0,0.030);
    pid_line->setParameters(0,0,0);
    const float target_speed = 6;

    int delay_ = 100;
    float alpha = 0, beta = 0;
    int pwm_left = 0, pwm_right = 0;

    actuators->updatePWM(pwm_left, pwm_right);
    while (true) {
        receive_msg_line();
        if (sensors->isObstacle()) break;
        if (instruction != 1) {
            actuators->stop();
            delay(delay_);
            continue;
        }
        sensors->qtraRead();
        alpha = line_control();
        sensors->encodersRead();
        beta = speed_control(target_speed);
        //Serial.println(String(alpha) + " - " + String(beta));
        pwm_left = beta + alpha;
        pwm_right = beta - alpha;
        //actuators->updatePWM(pwm_left, pwm_right);
        delay(delay_);
    }
    actuators->stop();
}


//============
void test_7() {
    /*
        turn at constant speed (inverse of forward) surélevé
    */

    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();

    int delay_ = 5;
    FrequencyState* freq_receiver = new FrequencyState(10);
    FrequencyState* freq_sender = new FrequencyState(20);
    FrequencyState* freq_obstacle = new FrequencyState(5);
    FrequencyState* freq_wave = new FrequencyState(0.01);
    FrequencyState* freq_speed_control = new FrequencyState(5);
    FrequencyState* freq_direction_control = new FrequencyState(25);

    float alpha = 0, beta = 0, target_speed = 0;
    int pwm_left = 0, pwm_right = 0;

    actuators->updatePWM(pwm_left, pwm_right);
    for (int seq_nb = 0; true; delay(delay_)) {
        if (freq_receiver->isNewState()) receive_msg_pid();
        if (freq_obstacle->isNewState() && sensors->isObstacle()) break;
        if (freq_direction_control->isNewState()) {
            if (pid_speed->getKp() > 0 && target_speed <= 5) target_speed += 0.20;
            sensors->encodersRead();
            alpha = forward_control();
            if (freq_speed_control->isNewState()) 
                beta = speed_control(target_speed);
        }

        pwm_left = beta + alpha;
        pwm_right = -(beta - alpha);
        actuators->updatePWM(pwm_left, pwm_right);

        if (freq_wave->isNewState()) {
            if (target_speed == 6) target_speed = 6;
            else target_speed = 6;
        }

        if (freq_sender->isNewState()) {
            Serial.println(String(seq_nb) + "/" + String(target_speed) + "/" +\
                String(sensors->getSpeed()) + "/" + String(sensors->getSpeedLeft())\
                + "/" + String(sensors->getSpeedRight()));
            seq_nb++;
        }
    }
    actuators->stop();
}


//============
void test_8() {
    /*
        Control turn at constant speed 
    */

    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();

    pid_speed->setParameters(12, 0, 0.022);
    pid_forward->setParameters(15, 0.07, 0.065);

    int delay_ = 5;
    FrequencyState* freq_obstacle = new FrequencyState(10);
    FrequencyState* freq_speed_control = new FrequencyState(5);
    FrequencyState* freq_direction_control = new FrequencyState(25);

    float alpha = 0, beta = 0, target_speed = 0;
    int pwm_left = 0, pwm_right = 0;

    actuators->updatePWM(pwm_left, pwm_right);
    for (int seq_nb = 0; true; delay(delay_)) {
        if (freq_obstacle->isNewState() && sensors->isObstacle()) break;
        if (freq_direction_control->isNewState()) {
            if (target_speed <= 5) target_speed += 0.20;
            sensors->encodersRead();
            alpha = forward_control();
            if (freq_speed_control->isNewState()) 
                beta = speed_control(target_speed);
        }

        pwm_left = beta + alpha;
        pwm_right = -(beta - alpha);
        actuators->updatePWM(pwm_left, pwm_right);
    }
    actuators->stop();
}

//============
void test_9() {
    /*
        Perturbation
        Determine max angle of perturbation
    */
}


//============
void test_10() {
    /*
        Full speed obstacle avoidance
    */
}


//============
void test_11() {
    /*
        Control uturn 
    */

    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();

    pid_speed->setParameters(12, 0, 0.022);
    pid_forward->setParameters(15, 0.07, 0.065);

    int delay_ = 5;
    FrequencyState* freq_obstacle = new FrequencyState(10);
    FrequencyState* freq_speed_control = new FrequencyState(5);
    FrequencyState* freq_direction_control = new FrequencyState(25);

    float alpha = 0, beta = 0, target_speed = 0;
    int pwm_left = 0, pwm_right = 0;

    unsigned long start_t = millis();
    actuators->updatePWM(pwm_left, pwm_right);
    for (int seq_nb = 0; true; delay(delay_)) {
        if (millis() > (start_t + 1200)) break; 
        if (freq_obstacle->isNewState() && sensors->isObstacle()) break;
        if (freq_direction_control->isNewState()) {
            if (target_speed <= 5) target_speed += 0.40;
            sensors->encodersRead();
            alpha = forward_control();
            if (freq_speed_control->isNewState()) 
                beta = speed_control(target_speed);
        }

        pwm_left = beta + alpha;
        pwm_right = -(beta - alpha);
        actuators->updatePWM(pwm_left, pwm_right);
    }
    actuators->stop();
}

//============
void test_12() {
    /*
        Control uturn 
    */

//    actuators->updatePWM(1, 1);
//    delay(2000);
    
    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();

    pid_speed->setParameters(12, 0, 0.022);
    pid_forward->setParameters(15, 0.07, 0.065);

    int delay_ = 5;
    FrequencyState* freq_obstacle = new FrequencyState(10);
    FrequencyState* freq_speed_control = new FrequencyState(5);
    FrequencyState* freq_direction_control = new FrequencyState(25);

    float alpha = 0, beta = 0, target_speed = 0;
    int pwm_left = 0, pwm_right = 0;

    unsigned long start_t = millis();
    actuators->updatePWM(pwm_left, pwm_right);
    for (int seq_nb = 0; true; delay(delay_)) {
        if (millis() > (start_t + 2000)) break; 
        if (freq_obstacle->isNewState() && sensors->isObstacle()) break;
        if (freq_direction_control->isNewState()) {
            if (target_speed <= 5) target_speed += 0.40;
            sensors->encodersRead();
            alpha = forward_control();
            if (freq_speed_control->isNewState()) 
                beta = speed_control(target_speed);
        }

        if (beta < 0) beta = 0;
        pwm_left = beta + alpha;
        if (pwm_left < 0) pwm_left = 0;
        pwm_right = -(beta - alpha);
        actuators->updatePWM(pwm_left, pwm_right);
    }
    actuators->stop();
}
