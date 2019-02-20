/*
    Flow test of sharp sensor.
*/


//============
void test(byte test_id) {
    switch (test_id) {
        case 1:
            test_1();
            break;
    }
}



//============
void test_1() {
    String header = "f;light_on;t;voltage";
    Serial.println(header);

    int time_step = 1000/f;
    for (unsigned int t = 0; t < T; t += time_step) {
        float voltage = sharp_voltage();
        if (is_obstacle()) analogWrite(led_sharp, 255);
        else analogWrite(led_sharp, 0);
        String measure = String(f) + ";" + String(light_on) + ";" + String(t) 
            + ";" + String(voltage); 
        Serial.println(measure);
        delay(time_step);
    } 
}
























