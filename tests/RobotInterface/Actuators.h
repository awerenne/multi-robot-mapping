#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include <Arduino.h>

#include "constants.h"


class Actuators {
 public:
    Actuators(unsigned char *pins);
    
    void FollowLine(int error, int previous_error);
    void OneInch();
    void Stop();
    void TurnLeft();
    void TurnRight();
    void TurnBack();

    float GetDistance();
    void ResetDistance();
    

 private:
    static const int kWheelRadius = WHEEL_RADIUS;  // In [cm]
    static const int kInterWheelDistance = INTER_WHEEL_DISTANCE;  // In [cm]
    static constexpr float kKp = KP;
    static constexpr float kKd = KD;  // parameters of PID controller
    static const int kNominalMotorVelocity = NOMINAL_LINEAR_VELOCITY;  // In [cm/[s]
    static const int kTimeFullTurn = TIME_360_ROTATION;  // In [s]
    static const int kRotationSpeed = SPEED_360_ROTATION;  // 
    static const int kTimeStepUpdate = TIME_STEP_UPDATE;
    unsigned char* pins;
    float distance;

    void Rotate(float angle);
    void UpdateMotorVelocities(float velocity_left, float velocity_right); // Write PWM
    void UpdateDistance(float velocity_left, float velocity_right);
};

#endif  // ACTUATORS_H_