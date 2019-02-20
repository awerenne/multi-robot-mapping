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



//============
void test_2() {
    // Test full range of speeds of motor
    for (int i = 0; i < 256; i++) {
        update_pwm_left(i);
        update_pwm_right(i);
        delay(50);
    }
    stop_motors();
}



//============
void test_3() {
    // Test rotation in both directions
    // Turn four times right
    for (int i = 0; i < 4; i++) {
        turn_right();
        delay(620);
    }

    // Turn four times left
    for (int i = 0; i < 4; i++) {
        turn_left();
        delay(620);
    }
}
