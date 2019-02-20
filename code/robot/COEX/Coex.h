#ifndef COEX_H_
#define COEX_H_

#include <Arduino.h>
#include "Actuators.h"
#include "Sensors.h"
#include "Messenger.h"


class PIDController;


//============
class Coex {
    private:
        Sensors* sensors;
        Actuators* actuators;
        Messenger* messenger;
        PIDController *pid_speed, *pid_forward, *pid_line;
        bool isIntersection();
        bool isObstacle();


    public: 
        Coex();
        byte followLine(const float& speed);
        byte forward(const float& speed);
        void oneInch(const float& speed);
        void turn90(const bool& clockwise, const float& speed);
        void turn180(const bool& clockwise, const float& speed);
        void turnLeft(const float& speed);
        void turnRight(const float& speed);
        void uturn(const float& speed);
        byte typeIntersection();
};   


//============
class PIDController {
    ;
};




#endif  // COEX_H_














