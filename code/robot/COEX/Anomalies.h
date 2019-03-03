#ifndef ANOMALIES_H_
#define ANOMALIES_H_


#include <Arduino.h>


//============
class Anomalies {
    public: 
        Anomalies();
        void reset();
        void start(float distance);
        void new_(float distance);
        void newLeft(bool new_anomaly);
        void newCenter(bool new_anomaly);
        void newRight(bool new_anomaly);
        bool isFinished();
        bool isIntersection();
        String getSummary();

    private:
        unsigned long counter, counter_left, counter_center, counter_right;
        float start_distance, x, p_left, p_center, p_front, p_right;
};


#endif  // ANOMALIES_H_














