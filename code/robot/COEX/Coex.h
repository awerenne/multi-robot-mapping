#ifndef COEX_H_
#define COEX_H_


#include <Arduino.h>
#include <Actuators.h>
#include <Sensors.h>
#include <Messenger.h>
#include <PidController.h>
#include <FrequencyState.h>
#include <Accelerator.h>
#include <Anomalies.h>


//============
class Coex {
    public: 
        Coex(const byte* pins_messenger, const byte* pins_actuators,
            const byte* pins_qta, const byte pin_sharp, const int baud_rate,
            const unsigned int* parameters_qta);
        void stop();
        bool availableMsg();
        void readMsg();
        void sendMsg(String msg) { messenger->sendMessage(msg); }
        int getMsgInstruction();
        void setTargetSpeed(const float& target_speed);
        void calibration() {sensors->manualCalibration();}
        void newLine(const float& target_speed, const bool& with_intersection);
        float followLine();
        void newForward(const float& target_speed);
        float forward();
        void forwardAlign(float x);
        byte turn90(const bool& clockwise, const float& target_speed);
        byte turn180(const bool& clockwise, const float& target_speed);
        byte typeIntersection();
        bool isAnomaly();
        bool isIntersection();
        void turnLeft(const float& speed);
        void turnRight(const float& speed);
        void uturn(const float& speed, byte typeIntersection);
        void turnAlign(const float& speed, byte intersection, byte type_intersection);

        Sensors* getSensors() { return this->sensors; }
        Actuators* getActuators() { return this->actuators; }

    private:
        Sensors* sensors;
        Actuators* actuators;
        Messenger* messenger;
        Anomalies* anom;
        PIDController *pid_speed, *pid_forward, *pid_line, *pid_responsive;
        FrequencyState *f_obstacle, *f_speed_ctrl, *f_dir_fwd_ctrl,
                *f_dir_line_ctrl, *f_acc, *f_msg, *f_rotation;
        Accelerator *acc_normal, *acc_rotation;
        int delay_;
        bool with_intersection;
        float progress_speed, target_speed, alpha, beta;

        byte turnTheta(const float& Theta, const bool& clockwise);
        float errorLine();
        float errorSpeed();
        float errorForward();
        float f(float x, float e, float a, float v_min);
};   



#endif  // COEX_H_














