void test_3_1() {
    String header = "num_samples_avg;dimming_level;f;light_on;h;t;position;vector_0;vector_1;vector_2;vector_3;vector_4;vector_5";
    Serial.println(header);
    
    int time_step = 1000/frequency;
    for (unsigned int t = 0; t < T; t += time_step) {
        QTRARead();
        String measure = String(num_samples_per_sensor) + ";" ;
        msg += String(dimming_level) + ";" ;
        msg += String(f) + ";" ;
        msg += String(light_on) + ";" ;
        msg += String(h) + ";" ;
        msg += String(t) + ";" ;
        msg += String(pos) + ";" ;
        for (unsigned int i = 0; i < NUM_SENSORS; ++i) {
          msg += String(sensor_values[i]);
          if (i+1 < NUM_SENSORS) msg += ";" ;
        }
    
        Serial.println(measure);
        delay(time_step);
    }
}
