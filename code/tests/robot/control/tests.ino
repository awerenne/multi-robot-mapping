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
    }
}



//============
void test_1() {
    /*
        Test bidirectional communication.
    */

    int delay_ = 50;
    while (true) {
        receive_msg_pid();
        if (instruction == 0 || sensors->isObstacle()) break;
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
        Kp left used battery = 65 
        Ki left used battery = 0,045
        Kp left charged battery = 40 
        Ki left charged battery = 0,025
        Kp right = 
        Ki right = 
    */

    pid_speed->reset();
    sensors->encodersReset();

    int delay_ = 50, iter = 0;
    int pwm = 0;
    float target_speed = 6, beta = 0, measured_speed;
    
    actuators->updatePWM(pwm, 0);
    while (true) {
        receive_msg_pid();
        if (instruction == 0 || sensors->isObstacle()) break;
        sensors->encodersRead();
        beta = speed_control_left(target_speed);
        pwm = beta;
        actuators->updatePWM(pwm, 0);
        measured_speed = sensors->getSpeedLeft();
        Serial.println(String(iter) + "/" + String(target_speed) + "/" + String(measured_speed));

        if (iter % 200 == 0) {
            if (target_speed == 6) target_speed = 7;
            else target_speed = 6;
        }
        
        delay(delay_);
        iter++;
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
        Kp_speed = 50
        Ki_speed = 0.025
        Kp_forward = 30
        Ki_forward = 0.020
    */

    pid_speed->reset();
    pid_forward->reset();
    sensors->encodersReset();

    int delay_ = 50, iter = 0;
    float alpha = 0, beta = 0, target_speed = 6;
    int pwm_left = 0, pwm_right = 0;

    actuators->updatePWM(pwm_left, pwm_right);
    while (true) {
        receive_msg_pid();
        if (instruction == 0 || sensors->isObstacle()) break;
        sensors->encodersRead();
        alpha = forward_control();
        beta = speed_control(target_speed);
        pwm_left = beta + alpha;
        pwm_right = beta - alpha;
        actuators->updatePWM(pwm_left, pwm_right);
        Serial.println(String(iter) + "/" + String(target_speed) + "/" + String(sensors->getSpeed())\
            + "/" + String(sensors->getSpeedLeft()) + "/" +\
            String(sensors->getSpeedRight()));

        if (iter % 200 == 0) {
            if (target_speed == 6) target_speed = 8;
            else target_speed = 6;
        }

        delay(delay_);
        iter += 1;
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

    pid_speed->setParameters(50, 0, 0.025);
    pid_forward->setParameters(30, 0, 0.02);
    const float target_speed = 6;

    int delay_ = 50;
    float alpha = 0, beta = 0;
    int pwm_left = 0, pwm_right = 0;

    actuators->updatePWM(pwm_left, pwm_right);
    while (true) {
        if (sensors->isObstacle()) break;
        sensors->encodersRead();
        alpha = forward_control();
        beta = speed_control(target_speed);
        pwm_left = beta + alpha;
        pwm_right = beta - alpha;
        actuators->updatePWM(pwm_left, pwm_right);
        delay(delay_);
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

    pid_speed->setParameters(0,0,0);
    pid_line->setParameters(0,0,0);
    const float target_speed = 5;

    int delay_ = 20;
    float alpha = 0, beta = 0;
    int pwm_left = 0, pwm_right = 0;

    actuators->updatePWM(pwm_left, pwm_right);
    while (true) {
        receive_msg_pid();
        if (instruction == 0 || sensors->isObstacle()) break;
        sensors->qtraRead();
        sensors->encodersRead();
        alpha = line_control();
        beta = speed_control(target_speed);
        pwm_left = beta + alpha;
        pwm_right = beta - alpha;
        actuators->updatePWM(pwm_left, pwm_right);
        delay(delay_);
    }
}


//============
void test_7() {
    /*
        turn at constant speed (inverse of forward)
    */
}


//============
void test_8() {
    /*
        Perturbation
        Determine max angle of perturbation
    */
}


//============
void test_9() {
    /*
        Full speed obstacle avoidance
    */
}
