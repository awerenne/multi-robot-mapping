/*
    Simple motor manipulation.
*/


//============
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


//============
void update_pwm_left(int pwm) {
    if (pwm >= 0) {
        digitalWrite(pin_IN3, HIGH);
        digitalWrite(pin_IN4, LOW); 
    }
    else {
        digitalWrite(pin_IN3, LOW);
        digitalWrite(pin_IN4, HIGH); 
    }
    analogWrite(pin_EN34, abs(pwm));
}


//============
void update_pwm_right(int pwm) {
    if (pwm >= 0) {
        digitalWrite(pin_IN1, HIGH);
        digitalWrite(pin_IN2, LOW); 
    }
    else {
        digitalWrite(pin_IN1, LOW);
        digitalWrite(pin_IN2, HIGH); 
    }
    analogWrite(pin_EN12, abs(pwm));
}


//============
void stop_motors() {
  digitalWrite(pin_IN1, HIGH);
  digitalWrite(pin_IN2, HIGH); 
  digitalWrite(pin_IN3, HIGH);
  digitalWrite(pin_IN4, HIGH); 
}





