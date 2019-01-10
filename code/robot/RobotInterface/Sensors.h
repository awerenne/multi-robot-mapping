#ifndef SENSORS_H_
#define SENSORS_H_

#include <Arduino.h>
#include <stdlib.h>
#include <QTRSensors.h>
#include <math.h>  // floor


class Sensors {
 public:
    Sensors(const byte* pins_qta, const unsigned int* parameters,
        const byte& pin_sharp);
    void Calibrate();
    void QTRARead();
    bool IsRoadLeft();
    bool IsRoadRight();
    bool IsRoadHere();
    bool IsAligned();
    int GetError();
    bool IsObstacle();
    
 private:
    QTRSensorsAnalog* qtra;
    int num_sensors;
    int position;
    unsigned int* sensor_values;
    char pin_sharp;

};

#endif  // SENSORS_H_