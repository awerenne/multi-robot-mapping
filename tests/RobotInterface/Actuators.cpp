#include "Actuators.h"


Actuators::Actuators(unsigned char *pins) { 
    this->pins = pins; 
}


void Actuators::FollowLine(int error, int previous_error) {
    // Compute PID value 
    float pid_value = (this->kKp * error) +
        (this->kKd * (error - previous_error));

    // Compute velocities after correction
    float velocity_left = this->kNominalMotorVelocity + pid_value;
    float velocity_right = this->kNominalMotorVelocity - pid_value;

    // Apply new velocities
    this->UpdateMotorVelocities(velocity_left, velocity_right);
}


void Actuators::OneInch() {
    this->UpdateMotorVelocities(0.1, 0.1);
    delay(500);
    this->Stop();
}


void Actuators::Stop() {
    this->UpdateMotorVelocities(0, 0);
}


void Actuators::TurnLeft() {
    this->Rotate(-0.25);
}


void Actuators::TurnRight() {
     this->Rotate(0.25);
}


void Actuators::TurnBack() {
     this->Rotate(-0.5);
}


float Actuators::GetDistance() {
    return this->distance;
}


void Actuators::ResetDistance() {
    this->distance = 0;
}


void Actuators::Rotate(float fraction_full_turn) {
    int clock_wise = fraction_full_turn > 0. ? 1 : -1; 

    // Rotate for certain amount of time
    float rotation_time = fraction_full_turn * this->kTimeFullTurn;
    this->UpdateMotorVelocities(clock_wise * this->kNominalMotorVelocity,
        -clock_wise * this->kNominalMotorVelocity);
    delay(rotation_time);
    this->Stop();
}


void Actuators::UpdateMotorVelocities(float velocity_left,
    float velocity_right) {
    float angular_velocity_left = velocity_left / this->kWheelRadius;
    float angular_velocity_right = velocity_right / this->kWheelRadius;
    float a,b; // Learn this mapping experimentally (measuring plot PWM - speed)
    if (angular_velocity_left >= 0) {
        digitalWrite(this->pins[0], HIGH);
        digitalWrite(this->pins[1], LOW); 
    }
    else {
        digitalWrite(this->pins[0], LOW);
        digitalWrite(this->pins[1], HIGH);
    }
    analogWrite(this->pins[2], (int) (a*angular_velocity_left + b));
    if (angular_velocity_right >= 0) {
        digitalWrite(this->pins[3], HIGH);
        digitalWrite(this->pins[4], LOW); 
    }
    else {
        digitalWrite(this->pins[3], LOW);
        digitalWrite(this->pins[4], HIGH);
    }
    this->pins[5] = a*angular_velocity_right + b;
    this->UpdateDistance(velocity_left, velocity_right);
}


void Actuators::UpdateDistance(float velocity_left, float velocity_right) {
    // Returns the norm of the single velocity vector taken for a point-model
    float distance_wheel_to_center = this->kInterWheelDistance/2.;
    float angle_center_to_left = atan(velocity_left/distance_wheel_to_center);
    float angle_center_to_right = atan(velocity_right/distance_wheel_to_center);
    float l1 = sqrt(pow(velocity_left, 2) + pow(angle_center_to_left, 2));
    float l2 = sqrt(pow(velocity_right, 2) + pow(angle_center_to_right, 2));
    float norm_velocity_from_center = sqrt(pow(l1, 2) + pow(l2, 2) +
            2*l1*l2*cos(angle_center_to_left + angle_center_to_right));
    this->distance += norm_velocity_from_center * this->kTimeStepUpdate;
}




