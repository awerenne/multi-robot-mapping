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
        case 5:
            test_5();
            break;
    }
}



//============
void test_1() {
    String header = "mode;num_samples_avg;f;t;error;v0;v1;v2;v3;v4;v5";
    Serial.println(header);
    String mode = "manual";
    int time_step = 1000/f;
    unsigned long prev_t = millis();
    float delta_t = 0;
    int k = 0;
    for (unsigned int t = 0; t < T; t += delta_t) {
        QTRARead();
        String measure = mode + ";" ;
        measure += String(num_samples_per_sensor) + ";" ;
        measure += String(f) + ";" ;
        measure += String(t) + ";" ;
        measure += String(get_error(pos)) + ";" ;
        for (unsigned int i = 0; i < num_sensors; ++i) {
          measure += String(sensor_values[i]);
          if (i+1 < num_sensors) measure += ";" ;
        }

        //k++;
        //if (k%10) 
        Serial.println(measure);
        delay(time_step);
        delta_t = ((float) millis() - prev_t)/1000.;
        prev_t = delta_t;
    }
    delay(5000);  
}



//============
void test_2() {
    String measure;
    while (true) {
        QTRARead();
        measure = String(isRoadLeft()) + " - ";
        measure += String(isRoadCenter()) + " - ";
        measure += String(isRoadRight());
        Serial.println(measure);
        delay(500);
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
    for (int i = 0; i < 12; i++) {
        Serial.println("iteration: " + String(i));

        if (i == 0) {
            led_value = true;
            digitalWrite(led_signal, HIGH);  // Do measures with light
        }
        if (i >= 2) {
            led_value = false;
            digitalWrite(led_signal, LOW);  // Do measures in the dark
        }

        msg = "Next test (lights on: " + String(led_value) + ")";
        Serial.println(msg);
        flicker_led(led_signal, 10, 800);
        
        qtra.read(sensor_values, QTR_EMITTERS_OFF); 
        msg = "\tambient-light: " + summarize_ambient_light();
        Serial.println(msg);

        flicker_led(led_signal, 10, 200);
        if (led_value) digitalWrite(led_signal, HIGH);
        else digitalWrite(led_signal, LOW);
        calibrate();

        msg = "\tcalib-min: " + vector2string(qtra.calibratedMinimumOn);
        Serial.println(msg);
        msg = "\tcalib-max: " + vector2string(qtra.calibratedMaximumOn);
        Serial.println(msg);     
    }
    delay(5000);         
}


//============
void test_4() {
    String header = "mode;num_samples_avg;f;t;position;v0;v1;v2;v3;v4;v5";
    Serial.println(header);
    String mode = "automatic";
    automatic_calibration();
    int time_step = 1000/f;
    for (unsigned int t = 0; t < T; t += time_step) {
        QTRARead();
        String measure = String(mode) + ";" ;
        measure += String(num_samples_per_sensor) + ";" ;
        measure += String(f) + ";" ;
        measure += String(t) + ";" ;
        measure += String(pos) + ";" ;
        for (unsigned int i = 0; i < num_sensors; ++i) {
          measure += String(sensor_values[i]);
          if (i+1 < num_sensors) measure += ";" ;
        }
    
        Serial.println(measure);
        delay(time_step);
    }
    delay(5000);  
}



//============
void test_5() {
    /*
        Test dimming level. consume less battery
    */
}
