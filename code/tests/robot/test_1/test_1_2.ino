/*
    Test full range of speeds of motor
*/


//============
void test_1_2() {
    for (int i = 0; i < 256; i++) {
        update_pwm_left(i);
        update_pwm_right(i);
        delay(50);
    }
    stop_motors();
}









