void test_1_1() {
    // Test motors separately (both directions)
    update_pwm_left(80);
    delay(5000);
    stop_motors();
    delay(1000);
    
    update_pwm_right(80);
    delay(5000);
    stop_motors();
    delay(1000);
    
    update_pwm_left(-80);
    delay(5000);
    stop_motors();
    delay(1000);
    
    update_pwm_right(-80);
    delay(5000);
    stop_motors();
    delay(1000);


    // Test several speeds for both motors together (forward)
    update_pwm_left(80);
    update_pwm_right(80);
    delay(5000);
    update_pwm_left(120);
    update_pwm_right(120);
    delay(5000);
    update_pwm_left(160);
    update_pwm_right(160);
    delay(5000);
    update_pwm_left(200);
    update_pwm_right(200);
    delay(5000);
    stop_motors();


    // Test several speeds for both motors together (backward)
    update_pwm_left(-80);
    update_pwm_right(-80);
    delay(5000);
    update_pwm_left(-120);
    update_pwm_right(-120);
    delay(5000);
    update_pwm_left(-160);
    update_pwm_right(-160);
    delay(5000);
    update_pwm_left(-200);
    update_pwm_right(-200);
    delay(5000);
    stop_motors();
}
