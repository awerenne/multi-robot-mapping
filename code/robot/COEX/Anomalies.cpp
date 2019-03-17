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
    x = (distance - start_distance);
    if (old_x <= 1 && x > 1) counter_center = 0;
    // if (old_x <= 0.8 && x > 1.5) counter_center = 0;
}


//============
void Anomalies::newLeft(bool new_anomaly) {
    float item = new_anomaly ? 1 : 0;
    if (x <= 1)
        p_left = (item + counter_left*p_left) / (++counter_left);  // Moving average
}


//============
void Anomalies::newCenter(bool new_anomaly) {
    float item = new_anomaly ? 1 : 0;
    if (x <= 1)
        p_center = (item + counter_center*p_center) / (++counter_center);  
    else if (x >= 1.5 && x <= 3)
        p_front = (item + counter_center*p_front) / (++counter_center);
}


//============
void Anomalies::newRight(bool new_anomaly) {
    float item = new_anomaly ? 1 : 0;
    if (x <= 1)
        p_right = (item + counter_right*p_right) / (++counter_right);  
}


//============
bool Anomalies::isFinished() {
    return x > 3;  
}


//============
bool Anomalies::isIntersection() {
    return (isBlackLeft() || isBlackRight() || !isBlackCenter() || !isBlackFront());
}


//============
byte Anomalies::typeIntersection() {
    bool bl = isBlackLeft(), br = isBlackRight(), bc = isBlackCenter(),
        bf = isBlackFront();
    if (bl && br && bf) return 0;
    if (bl && !br && bf) return 1;
    if (!bl && br && bf) return 2;
    if (bl && br && !bf) return 3;
    if (bl && !br && !bf) return 4;
    if (!bl && br && !bf) return 5;
    return 6;
}


//============
bool Anomalies::isBlackLeft() {
    return p_left >= 0.4;
}


//============
bool Anomalies::isBlackRight() {
    return p_right >= 0.4;
}


//============
bool Anomalies::isBlackCenter() {
    return p_center > 0.5;
}


//============
bool Anomalies::isBlackFront() {
    return p_front > 0.5;
}


//============
String Anomalies::getSummary() {
    return String(counter_left) + ": " + String(p_left) + ","\
            + String(p_center) + "," + String(p_right) + "/ " +\
            String(counter_center) + ": " + String(p_front);
}
























