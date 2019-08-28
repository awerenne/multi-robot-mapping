/*
    Description.
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
    /* 
        Check correct angular velocity computation. Possible sources of 
        errors: noise in encoder (high frequencies), wrong gearbox ratio

        Test:  camera to capture slowmotion of rotation of wheel. Rotate wheel 
        for certain pwm, compute at each time step the angular velocity and
        multiply it by time step, so that we obtain revolution. At end of 
        test we check if total number of revolution corresponds at those observed
        by camera. Do test for both wheels.
    */ 
    
    int delay_ = 50, T = 4000;
    int pwm_left = 75, pwm_right = 75;
    float rev_left = 0, rev_right = 0;
    encoder_left->reset();
    encoder_right->reset();

    encoder_left->newTimeStep();
    encoder_right->newTimeStep();
    actuators->updatePWM(pwm_left, pwm_right);
    
    for (unsigned long i = 0; i < T; i += encoder_left->deltaTime()) {
        delay(delay_);  
        encoder_left->newTimeStep();
        encoder_right->newTimeStep();   
        float t_left = encoder_left->deltaTime();
        float t_right = encoder_right->deltaTime();  
        float w_left = angular_velocity(encoder_left->deltaCounter(), t_left);
        float w_right = angular_velocity(encoder_right->deltaCounter(), t_right);
        rev_left += (w_left * t_left / 1000) / (2 * 3.1415);
        rev_right += w_right * t_right/1000 / (2 * 3.1415);
    }

    actuators->stop();
    Serial.println("Rev. left: " + String(rev_left) + "- Rev. right: " + String(rev_right)); 
    delay(2000);  // Wait to be sure results are printed before quitting 
}



//============
void test_2() {
    /* 
        Test relationship between speed and pwm for each motor.
    */ 
    
    int delay_ = 200;
    Serial.println("pwm;speed_left;speed_right");
    encoder_left->reset();
    encoder_right->reset();

    actuators->updatePWM(-255, -255);
    delay(3000);

    encoder_left->newTimeStep();
    encoder_right->newTimeStep();    
    for (int pwm = -255; pwm <= 255; pwm += 2) { 
        actuators->updatePWM(pwm, pwm);
        delay(delay_);
        encoder_left->newTimeStep();
        encoder_right->newTimeStep(); 
        float t_left = encoder_left->deltaTime();
        float t_right = encoder_right->deltaTime();  
        float v_left = angular_velocity(encoder_left->deltaCounter(), t_left);
        float v_right = angular_velocity(encoder_right->deltaCounter(), t_right);
        if (pwm < 0) {
            v_left *= -1;
            v_right *= -1;
        }
        Serial.println(String(pwm) + ";" + String(v_left) + ";" + String(v_right));
    }
    actuators->stop();
}



//============
void test_3() {
    /* 
        Apply robot on straight line. Stop it after 10 seconds. Display mean speed. And by checking the travelled distance, check
        if there is an error.
    */ 
    
    int delay_ = 50;
    encoder_left->reset();
    encoder_right->reset();

    actuators->updatePWM(65, 75);  // Approx straight line
    delay(5000);
    actuators->stop();
    
    encoder_left->newTimeStep();
    encoder_right->newTimeStep();
     
    float t_left = encoder_left->deltaTime();
    float t_right = encoder_right->deltaTime();  
    float v_left = angular_velocity(encoder_left->deltaCounter(), t_left);
    float v_right = angular_velocity(encoder_right->deltaCounter(), t_right);
    float v = (v_left+v_right)/2;
    bluetooth->println(String(v) + "-" + String((t_left+t_right)/2));
    actuators->stop();
}

















