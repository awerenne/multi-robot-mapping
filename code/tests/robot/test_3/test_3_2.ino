/*
    Simple intersection detection.
*/


//============
void test_3_2() {
    String measure;
    while (true) {
        QTRARead();
        measure = String(isRoadLeft()) + " ";
        measure += String(isRoadMiddle()) + " ";
        measure += String(isRoadRight());
        Serial.println(measure);
        delay(1000);
    }
}



//============
bool isRoadLeft() {
    return (sensor_values[num_sensors-1] > 200);
}


//============
bool isRoadMiddle() {
    return (pos < (num_sensors-1)*1000*0.7 && pos > (num_sensors-1)*1000*0.3);
}


//============
bool isRoadRight() {
    return (sensor_values[0] > 200);
}






