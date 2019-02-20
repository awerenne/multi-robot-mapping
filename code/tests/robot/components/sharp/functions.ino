

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
float sharp_voltage() {
    int value = analogRead(pin_sharp);  // Value scaled between 0 and 1023 
    return ((float) value)/1024.;       
}


//============
bool is_obstacle() {
    if (sharp_voltage() < 0.4) return true;
    return false;
}
