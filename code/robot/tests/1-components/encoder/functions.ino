/*
    Description.
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
void update_counter_left() { encoder_left->incrementCounter(); }



//============
void update_counter_right() { encoder_right->incrementCounter(); }



//============
float angular_velocity(float counts, float t) {
    float gearbox_ration = 150.58;
    float counts_per_revolution = 6;  // on one working channel
    float n_revolutions = counts / (gearbox_ration * counts_per_revolution);
    float frequency = n_revolutions / t;  // n_rev per millis
    frequency *= 1000;  // n_rev per sec
    return 2. * 3.1415 * frequency;
}



//============
float linear_velocity(float counts, float t) {
    float radius_wheel = 0.8;  // cm
    return angular_velocity(counts, t) * radius_wheel;
}














