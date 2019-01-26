#ifndef SENSORS_H_
#define SENSORS_H_


#include <Arduino.h>
#include <QTRSensors.h>

#include <stdlib.h>  // Check why?
#include <math.h>  // floor

#include "Actuators.h"

class Actuators;  // To avoid problem of recursive definition

class Sensors {
 
 public:
    Sensors(const byte* pins_qta, const unsigned int* parameters,
        const byte& pin_sharp);

    void QTRARead();
    void Calibrate();
    bool IsRoadLeft();
    bool IsRoadRight();
    bool IsRoadHere();
    bool IsAligned(bool clockwise);
    int GetError();
    void AutomaticCalibrate();

    QTRSensorsAnalog* GetQTRObject();  // For tests 
    unsigned int* GetQTRValues();  // For tests 
    unsigned int GetQTRNumberSensors();  // For tests

    bool IsObstacle(Actuators* actuators);

    void InitEncoders();
    void StopEncoders();
    float GetSpeedLeft();
    float GetSpeedRight();
    float GetDistance();
    void IncrementLeft();
    void IncrementRight();

    bool IsIntersection(Actuators* actuators);
    byte TypeIntersection(Actuators* actuators);

 private:
    QTRSensorsAnalog* qtra;
    int num_sensors;
    int position;
    unsigned int* sensor_values;

    byte pin_sharp;

    bool counter_active;
    volatile unsigned long counter_left;
    volatile unsigned long counter_right;
    unsigned long previous_time;
    const float alpha = 111.271;
};

#endif  // SENSORS_H_