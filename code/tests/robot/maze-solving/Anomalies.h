#ifndef ANOMALIES_H_
#define ANOMALIES_H_


#include <Arduino.h>


//============
class Anomalies {
    public: 
        Anomalies();
        void reset();
        void start(float distance);
        void new_();
        void newLeft(bool new_anomaly, float distance);
        void newCenter(bool new_anomaly, float distance);
        void newRight(bool new_anomaly, float distance);
        bool isFinished();
        bool isIntersection();
        String getSummary();

    private:
        unsigned long counter, counter_left, counter_center, counter_right;
        float start_distance, x;
};


#endif  // ANOMALIES_H_














