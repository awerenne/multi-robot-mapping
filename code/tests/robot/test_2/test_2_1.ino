void test_2_1() {
    // Flow test of sharp sensor (TODO find better explanation)
    String header = "f;light_on;t;voltage";
    Serial.println(header);

    int time_step = 1000/f;
    for (unsigned int t = 0; t < T; t += time_step) {
        int value = analogRead(pin_sharp);  // Value scaled between 0 and 1023 corresponding to 0 and VCC
        float voltage = ((float) value)/1024.;  // Scale to measured voltage by sharp sensor
        
        // Fire led on if object detected
        if (voltage < 0.4)
            analogWrite(led_sharp, 255);
        else
            analogWrite(led_sharp, 0);
        
        // Display measure on Serial port 
        String measure = String(f) + ";" + String(light_on) + ";" + String(t) + ";" + String(voltage); 
        Serial.println(measure);
        delay(time_step);
    } 
}
