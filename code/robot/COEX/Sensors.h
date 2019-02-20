#ifndef SENSORS_H_
#define SENSORS_H_

#include <Arduino.h>
#include <QTRSensors.h>
#include <stdlib.h>  
#include <math.h>


class EncoderState; 


//============
class Sensors {
 
    public:
        Sensors(const byte* pins_qta, const unsigned int* parameters,
            const byte& pin_sharp);
        void qtraRead();
        void manualCalibration();
        void automaticCalibration();
        bool isRoadLeft();
        bool isRoadRight();
        bool isRoadCenter();
        bool isAligned();
        int getError();
        bool isObstacle();
        float getSpeed();
        float getSpeedLeft();
        float getSpeedRight();
        float getDistance();
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



//============
class EncoderState {
    private:
        volatile unsigned long counter;
        unsigned long prev_counter, current_counter;
        unsigned long prev_time, current_time;

    public: 
        EncoderState();
        void resetCounter();
        void newTimeStep();
        void incrementCounter();
        float deltaTime();
        float deltaCounter();
};




#endif  // SENSORS_H_