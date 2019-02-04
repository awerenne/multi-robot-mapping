/*
    Calibration. The problem is I don't know how to set calibrated values 
    in class (Go change directly in Library of Pololu!!!!). 

    Detect with ambient light which are the calibrated values.

*/


//============
void test_8_1() {
    QTRSensorsAnalog* qtra = sensors->GetQTRObject();
    unsigned num_sensors = sensors->GetQTRNumberSensors();
    unsigned int* sensor_values = sensors->GetQTRValues();
    String msg;

    // Perform 10 measures (5 in the dark and 5 in light environment)
    for (int i = 0; i < 10; i++) {
        Serial.println("Iteration " + String(i));

        // Warn tester ambient light measure is going to start (low frequency)
        flicker_led(led_signal, 10, 800);
        digitalWrite(led_signal, LOW); 
        
        // Read ambient light
        qtra->read(sensor_values, QTR_EMITTERS_OFF); 
        msg = "A;";  // Ambient
        msg += summarize_ambient_light(sensor_values, num_sensors);
        Serial.println(msg);

        // Warn tester calibration is going to start (high frequency)
        flicker_led(led_signal, 10, 200);
        digitalWrite(led_signal, LOW); 

        sensors->Calibrate();

        unsigned int *calibratedMinimumOn = qtra->calibratedMinimumOn;
        msg = "C1;" + vector2string(calibratedMinimumOn, num_sensors);
        Serial.println(msg);

        unsigned int *calibratedMaximumOn = qtra->calibratedMaximumOn;
        msg = "C2;" + vector2string(calibratedMaximumOn, num_sensors);
        Serial.println(msg);

        // unsigned int *calibratedMinimumOff = qtra->calibratedMinimumOff;
        // msg = "C3;" + vector2string(calibratedMinimumOff, num_sensors);
        // Serial.println(msg);

        // unsigned int *calibratedMaximumOff = qtra->calibratedMaximumOff; 
        // msg = "C4;" + vector2string(calibratedMaximumOff, num_sensors);
        // Serial.println(msg);        
    }
             
}


//============
String summarize_ambient_light(unsigned int* sensor_values, int num_sensors) {
    String msg;
    unsigned int min, max;
    get_min_max(sensor_values, num_sensors, min, max);
    msg += String(min) + ";" + String(max);
    return msg;  
}


//============
String vector2string(unsigned int* sensor_values, int num_sensors) {
    String msg;
    for (int i = 0; i < num_sensors; i++) {
        msg += String(sensor_values[i]);
        if (i < num_sensors-1)
            msg += ";";
    }   
    return msg;  
}


//============
void get_min_max(unsigned int* v, int len, unsigned int& min, unsigned int& max) {
    min = 10000;  // Value can not be superior due to QTR specs
    max = 0;
    for (int i = 0; i < len; i++) {
        if (v[i] < min) min = v[i];
        if (v[i] > max) max = v[i];
    }
} 


























