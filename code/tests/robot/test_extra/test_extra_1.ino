/*
    Description.
*/


void test_extra_1() {
    // Test voltage varying
    // How is pwm-speed affected?
    // What is the threshold when not enough battery anymore and ask recharge
    // What strategy to decide threshold
    // How to determine which curve we are on (pwm-speed) 

    float pwm_left, pwm_right;
    float target_speed = 0.022;
    float measured_speed_left = 0, measured_speed_right = 0;
    float error_left = 0, previous_error_left = 0, accumulated_error_left = 0;
    float error_right = 0, previous_error_right = 0, accumulated_error_right = 0;
    float kp = 3500, kd = 1500, ki = 0;
    float pid_value_left, pid_value_right;
    unsigned int t = 0;

    Serial.println("t;target_speed;measured_speed_left;measured_speed_right");
    for (int iteration = 0; true; iteration++) {
        sensors->InitEncoders();
        delay(50);
        t += 20;
        measured_speed_left = sensors->GetSpeedLeft();
        measured_speed_right = sensors->GetSpeedRight();
        sensors->StopEncoders();
        
        error_left = target_speed - measured_speed_left;
        error_right = target_speed - measured_speed_right;
        pid_value_left = (kp * error_left) + (kd * (error_left - previous_error_left)) + (ki * accumulated_error_left);
        pid_value_right = (kp * error_right) + (kd * (error_right - previous_error_right)) + (ki * accumulated_error_right);
        previous_error_left = error_left;
        previous_error_right = error_right;

        pwm_left = to_pwm_left(target_speed) + pid_value_left;
        pwm_right = to_pwm_right(target_speed) + pid_value_right;
        actuators->UpdatePWM(pwm_left, pwm_right); 
        
        if (iteration % 10 == 0)
            Serial.println(String(t)+";"+String(1000*target_speed)+";"+String(1000*measured_speed_left)+";"+String(1000*measured_speed_right));
    }
    
}


float to_pwm_left(float speed) {
    return 3196.25 * speed;
}

float to_pwm_right(float speed) {
    return 3373.83 * speed;
}