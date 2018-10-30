//
// Created by aurelien-werenne on 19/09/2018.
//

#ifndef ACTUATORS_H_
#define ACTUATORS_H_


#include <string>
#include <sys/time.h>  // gettimeofday()


#include "Sensors.h"


class Actuators {
 public:
    Actuators(Sensors sensors);
    void GoLeft();
    void GoStraight();
    void OneInch();
    void GoRight();
    void GoBack();
    void Stop();
    float GetVelocity();  // unit: cm/s
    

 private:
    Sensors sensors;
    int current_error_value, previous_error_value;
    const float kPI;
    const float kWheelRadius;  // In [cm]
    const float kInterWheelDistance;  // In [cm]
    const float kKp, kKd;  // parameters of PID controller
    const float kNominalMotorVelocity;  // In [cm/[s]
    const float kTimeFullTurn;  // In [s]


    void FollowLine();
    void UpdateErrorValues();
    void TurnLeft();
    void TurnRight();
    void TurnBack();
    void Rotate(float angle);

    // Low-level functions
    float GetLinearVelocityLeftWheel();  // unit: cm/s
    float GetLinearVelocityRightWheel();  // unit: cm/s
    void UpdateMotorVelocities(float velocity_left, float velocity_right);
};

#endif  // ACTUATORS_H_