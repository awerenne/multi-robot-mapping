#ifndef COEX_H_
#define COEX_H_


#include <Arduino.h>
#include <Actuators.h>
#include <Sensors.h>
#include <Messenger.h>
#include <PidController.h>
#include <FrequencyState.h>
#include <Accelerator.h>



//============
class Coex {
    public: 
        Coex(const byte* pins_messenger, const byte* pins_actuators,
            const byte* pins_qta, const byte pin_sharp, const int* parameters_blt,
            const unsigned int* parameters_qta);
        void setTargetSpeed(const float& target_speed);
        void calibration();
        void newLine(const float& target_speed, const bool& with_distance);
        byte followLine();
        void newForward(const float& target_speed);
        byte forward();
        void oneInch(const float& speed);
        byte turn90(const bool& clockwise, const float& target_speed);
        byte turn180(const bool& clockwise, const float& target_speed);
        void turnLeft(const float& speed);
        void turnRight(const float& speed);
        void uturn(const float& speed);
        byte typeIntersection();

    private:
        Sensors* sensors;
        Actuators* actuators;
        Messenger* messenger;
        PIDController *pid_speed, *pid_forward, *pid_line;
        FrequencyState *f_obstacle, *f_speed_ctrl, *f_dir_fwd_ctrl,
                *f_dir_line_ctrl, *f_acc;
        Accelerator *acc_normal, *acc_rotation;
        bool with_distance;
        int delay_;
        float progress_speed, target_speed;

        bool isIntersection();
        byte turnTheta(const float& Theta, const bool& clockwise);
        float errorLine();
        float errorSpeed();
        float errorForward();

};   



#endif  // COEX_H_














