#include "Anomalies.h"


//============
Anomalies::Anomalies() {
    reset();
}


//============
void Anomalies::reset() {
    counter_left = 0;
    counter_center = 0;
    counter_right = 0;
    start_distance = 0;
    p_left = 0;
    p_center = 0;
    p_right = 0;
    p_front = 0;
    x = 0;
}


//============
void Anomalies::start(float distance) {
    start_distance = distance;
    x = 0;
}


//============
void Anomalies::new_(float distance) {
    float old_x = x;
    x += (distance - start_distance);
    if (old_x <= 0.6 && x > 0.6) counter_center = 0;
}


//============
void Anomalies::newLeft(bool new_anomaly) {
    float item = new_anomaly ? 1 : 0;
    if (x <= 0.6)
        p_left = (item + counter_left*p_left) / (++counter_left);  // Moving average
}


//============
void Anomalies::newCenter(bool new_anomaly) {
    float item = new_anomaly ? 1 : 0;
    if (x <= 0.6)
        p_center = (item + counter_center*p_center) / (++counter_center);  
    else if (x >= 1 && x <= 1.6)
        p_front = (item + counter_center*p_front) / (++counter_center);
}


//============
void Anomalies::newRight(bool new_anomaly) {
    float item = new_anomaly ? 1 : 0;
    if (x <= 0.6)
        p_right = (item + counter_right*p_right) / (++counter_right);  
}


//============
bool Anomalies::isFinished() {
    return x > 1.6;  // To change to 2.4 when factor 2 fixed
}


//============
bool Anomalies::isIntersection() {
    return (p_left > 0.5 || p_center < 0.5 || p_right > 0.5 || p_front < 0.5);
}


//============
String Anomalies::getSummary() {
    return String(counter_left) + ": " + String(p_left) + ","\
            + String(p_center) + "," + String(p_right) + "/ " +\
            String(counter_center) + ": " + String(p_front);
}
























