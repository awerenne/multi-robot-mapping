void test_5_3() {
    // Cycle through full range of PWM 
    // Compute speed and then save the couple (PWM, speed) which can be used
    // to approximate the function F(PWM) = speed
    
    unsigned int delay_ = 100;
    float speed_left = 0, speed_right = 0;
    previous_time_left = millis();
    previous_time_right = millis();

    String header = "t;pwm_left;speed_left;pwm_right;speed_right";
    Serial.println(header);
    
    for (int i = 0; i < 10; i++) {  // Number of cycles
        int start_pwm, end_pwm, adder;

        // Alternate between increasing and decreasing pwm,
        // from 0 to 255 then from 255 to 0
        if (i%2) {
            start_pwm = 0;
            end_pwm = 255;
            adder = 1;
        }
        else {
            start_pwm = 255;
            end_pwm = 0;
            adder = -1;
        }

        // One cycle
        for (int pwm = start_pwm; pwm != end_pwm; pwm += adder) {
            counter_left = 0;
            previous_time_left = millis();
            counter_right = 0;
            previous_time_right = millis();
            
            // Increase speed of motors
            actuators->UpdatePWM(pwm, pwm);
            delay(delay_);
    
            // Compute speed
            speed_left = alpha * counter_left /
                ((float) (millis() - previous_time_left));
            speed_right =  alpha * counter_right /
                ((float) (millis() - previous_time_right));

            // Display measure to Serial
            String measure = String(previous_time_left) + ";" ;
            measure += String(pwm) + ";" ;
            measure += String(speed_left) + ";" ;
            measure += String(speed_right);
            Serial.println(measure);
            
        }
    }
    actuators->Stop();
}
