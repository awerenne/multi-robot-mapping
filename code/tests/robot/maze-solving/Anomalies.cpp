#include "Anomalies.h"


//============
Anomalies::Anomalies() {
    reset();
}


//============
void Anomalies::reset() {
    counter = 0;
    counter_left = 0;
    counter_center = 0;
    counter_right = 0;
    start_distance = 0;
    x = 0;
}


//============
void Anomalies::start(float distance) {
    start_distance = distance;
    x = start_distance;
}


//============
void Anomalies::new_() {
    counter++;
}


//============
void Anomalies::newLeft(bool new_anomaly, float distance) {
    x += (distance - start_distance);
    if (new_anomaly) counter_left++;
}


//============
void Anomalies::newCenter(bool new_anomaly, float distance) {
    x += (distance - start_distance);
    if (new_anomaly) counter_center++;
}


//============
void Anomalies::newRight(bool new_anomaly, float distance) {
    x += (distance - start_distance);
    if (new_anomaly) counter_right++;
    counter++;
}


//============
bool Anomalies::isFinished() {
    return x >= 1.2;
}


//============
bool Anomalies::isIntersection() {
    float prop_left = ((float) counter_left)/((float) counter);
    float prop_center = ((float) counter_center)/((float) counter);
    float prop_right = ((float) counter_right)/((float) counter);
    return (prop_left > 0.5 || prop_center > 0.5 || prop_right > 0.5);
}


//============
String Anomalies::getSummary() {
    float prop_left = ((float) counter_left)/((float) counter);
    float prop_center = ((float) counter_center)/((float) counter);
    float prop_right = ((float) counter_right)/((float) counter);
    return String(counter) + "-" + String(prop_left) + "-"\
            + String(counter_center) + "-" + String(counter_right);
}
























