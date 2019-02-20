/*
    ...
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
        case 4:
            test_4();
            break;
    }
}



//============
void test_1() {
    String header = "num_samples_avg;dimming_level;f;light_on;h;t;position;vector_0;vector_1;vector_2;vector_3;vector_4;vector_5";
    Serial.println(header);
    
    int time_step = 1000/f;
    for (unsigned int t = 0; t < T; t += time_step) {
        QTRARead();
        String measure = String(num_samples_per_sensor) + ";" ;
        measure += String(dimming_level) + ";" ;
        measure += String(f) + ";" ;
        measure += String(light_on) + ";" ;
        measure += String(h) + ";" ;
        measure += String(t) + ";" ;
        measure += String(pos) + ";" ;
        for (unsigned int i = 0; i < num_sensors; ++i) {
          measure += String(sensor_values[i]);
          if (i+1 < num_sensors) measure += ";" ;
        }
    
        Serial.println(measure);
        delay(time_step);
    }
}



//============
void test_2() {
    String measure;
    while (true) {
        QTRARead();
        measure = String(isRoadLeft()) + " ";
        measure += String(isRoadCenter()) + " ";
        measure += String(isRoadRight());
        Serial.println(measure);
        delay(1000);
    }
}



//============
void test_3() {
    /*
        Test values that can help do an automatic calibration.
        Measure ambient light and measure for each sensor the min and max
        values while having a line flow beneath it.
    */

    QTRARead();
    String msg;

    bool led_value = false;
    for (int i = 0; i < 4; i++) {
        Serial.println("iteration: " + String(i));

        if (i == 0) {
            flicker_led(led_signal, 10, 800);
            led_value = true;
            digitalWrite(led_signal, HIGH);  // Do measures with light
        }
        if (i >= 2) {
            flicker_led(led_signal, 10, 800);
            led_value = false;
            digitalWrite(led_signal, LOW);  // Do measures in the dark
        }
        
        qtra.read(sensor_values, QTR_EMITTERS_OFF); 
        msg = "ambient-light: " + summarize_ambient_light();
        Serial.println(msg);

        flicker_led(led_signal, 10, 200);
        if (led_value) digitalWrite(led_signal, HIGH);
        else digitalWrite(led_signal, LOW);
        calibrate();

        msg = "calib-min: " + vector2string(qtra.calibratedMinimumOn);
        Serial.println(msg);
        msg = "calib-max: " + vector2string(qtra.calibratedMaximumOn);
        Serial.println(msg);     
    }
             
}


//============
void test_4() {
    /*
        Test dimming level. consume less battery
    */
}

























