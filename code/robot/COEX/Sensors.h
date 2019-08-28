#ifndef SENSORS_H_
#define SENSORS_H_


#include <Arduino.h>
#include <stdlib.h>  
#include <math.h>
#include <QTRSensors.h>
#include <EncoderState.h>


#include "Actuators.h"

//============
class Sensors {
    public:
        Sensors(const byte* pins_qta, const unsigned int* parameters,
            const byte& pin_sharp);
        void manualCalibration();
        void oneStepCalibration();
        void automaticCalibration(Actuators* actuators);
        void qtraRead();
        int getError();
        unsigned int* getValues() { return sensor_values; }
        bool isRoadLeft();
        bool isRoadRight();
        bool isRoadCenter();
        bool isRoadFront();
        bool isAligned();
        bool isObstacle();
        float getSpeed();
        float getSpeedLeft();
        float getSpeedRight();
        float getDistance();
        float getCounterDeltaTime();
        void encodersReset();
        void incrementLeft();
        void incrementRight();
        void encodersRead();

    private:
        EncoderState* encoder_left;
        EncoderState* encoder_right;
        QTRSensorsAnalog* qtra;
        unsigned int* sensor_values;
        int num_sensors, position;
        byte pin_sharp;

        bool ambientIsDark();
        void setCalibration(unsigned int* calib_min, unsigned int* calib_max);
        float sharpVoltage();
        float angularVelocity(const float& counts, const float& time);
        float linearVelocity(const float& counts, const float& time);
        
};




#endif  // SENSORS_H_