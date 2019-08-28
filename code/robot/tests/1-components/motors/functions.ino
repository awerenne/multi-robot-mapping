/* 
    ..
*/


//============
void flicker_led(byte led, unsigned int n, unsigned int delay_) {
    for (int i = 0; i < n; i++) {
        digitalWrite(led, LOW);
        delay(delay_);
        digitalWrite(led, HIGH);
        delay(delay_);
    }
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





























