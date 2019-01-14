/*
    Flow test on line sensors.
*/


//============
void test_3_1() {
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
void QTRARead() {
    pos = qtra.readLine(sensor_values); 
}
