#ifndef SENSORS_H_
#define SENSORS_H_

#include <Arduino.h>
#include <stdlib.h>

#include <QTRSensors.h>


class Sensors {
 public:
    Sensors(QTRSensorsRC* qtrrc, int num_sensors, char pin_sharp);
    void QTRRead();
    bool IsRoadLeft();
    bool IsRoadRight();
    bool IsRoadMiddle();
    int GetError();
    float SharpRead();
    
 private:
    QTRSensorsRC* qtrrc;
    int num_sensors;
    int position;
    unsigned int* sensor_values;
    char pin_sharp;

};

#endif  // SENSORS_H_