/*
    Test rotation in both directions.
*/


//============
void test_1_3() {
    // Turn four times to do full turn - adjust delay if needed
    for (int i = 0; i < 4; i++) {
        turn_right();
        delay(620);
    }

    // Turn four times to do full turn - adjust delay if needed
    for (int i = 0; i < 4; i++) {
        turn_left();
        delay(620);
    }
}


//============
void turn_right() {
  update_pwm_left(65);
  update_pwm_right(-65);
  delay(720);
  stop_motors();
}


//============
void turn_left() {
  update_pwm_left(-65);
  update_pwm_right(65);
  delay(720);
  stop_motors();
}









