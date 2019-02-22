/*
    Flow test on line sensors.
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
void calibrate() {
    for (int i = 0; i < 400; i++) qtra.calibrate(); 
}



//============
void QTRARead() {
    pos = qtra.readLine(sensor_values); 
}



//============
bool isRoadLeft() {
    return (sensor_values[num_sensors-1] > 600);
}



//============
bool isRoadCenter() {
    float mean = 0;
    for (int i = 1; i < 5; i++) mean += sensor_values[i];
    mean /= 4; 
    return (mean > 600);
}



//============
bool isRoadRight() {
    return (sensor_values[0] > 600);
}



//============
String summarize_ambient_light() {
    String msg;
    unsigned int min, max;
    get_min_max(sensor_values, num_sensors, min, max);
    msg += String(min) + ";" + String(max);
    return msg;  
}


//============
String vector2string(unsigned int* values) {
    String msg;
    for (int i = 0; i < num_sensors; i++) {
        msg += String(values[i]);
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


//============
bool ambient_is_dark() {
    qtra.read(sensor_values, QTR_EMITTERS_OFF); 
    int min_value = 1000;
    for (int i = 0; i < num_sensors; i++) 
        if (sensor_values[i] < min_value) min_value = sensor_values[i];
    if (min_value >= 400) return true;
    return false;
}


//============
void set_calibration(unsigned int* calib_min, unsigned int* calib_max) {
    qtra.calibratedMinimumOn = calib_min;
    qtra.calibratedMaximumOn = calib_max;
}      


//============
void automatic_calibration() {
    unsigned int calib_min_dark[6] = {100,100,100,100,100,100};  
    unsigned int calib_max_dark[6] = {750,750,750,750,750,750}; 
    unsigned int calib_min_light[6] = {100,100,100,100,100,100};  
    unsigned int calib_max_light[6] = {600,600,600,600,600,600};  
    if (ambient_is_dark())
        set_calibration(calib_min_dark, calib_max_dark);
    else
        set_calibration(calib_min_light, calib_max_light);
}

































