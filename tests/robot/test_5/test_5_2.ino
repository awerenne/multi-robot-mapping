void test_5_2() {
    // Test to check if encoder is worker well,
    // counter must increaser when speed increases
    // n_revolutions = counter / 903.48
    // distance = n_revolutions * 2 * PI * R = 0.111271 * counter [mm]
    // speed = distance / time = 111.271 * (counter/time_milliseconds) [mm/s]
    
    float speed_left = 0, speed_right = 0;
    unsigned int delay_ = 1000;
    previous_time_left = millis();    
    
    for (int pwm = 50; pwm <= 255; pwm += 5) { 
        counter_left = 0;
        previous_time_left = millis();
        counter_right = 0;
        previous_time_right = millis();
        
        // Increase speed of motors
        actuators->UpdatePWM(pwm, pwm);
        delay(delay_);

        // Compute speed
        speed_left = alpha * counter_left / ((float) (millis() - previous_time_left));
        speed_right =  alpha * counter_right / ((float) (millis() - previous_time_right));
        Serial.println(String(pwm) + " (PWM) - " + String(speed_left) + " (speed left) - " + String(speed_right) + " (speed right)");  
    }
    actuators->Stop();
}
