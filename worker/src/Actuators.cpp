

#include "../include/Actuators.h"

//  Initialize all parameters from file of Sensors & Actuators
//  Log to screen (stream passed in parameters)

void Actuators::Actuators(Sensors sensors) : kPI(3.1415) {
    this->sensors = sensors;
    this->kInterWheelDistance = 3.;
    this->current_error_value = 0;
    this->previous_error_value = 0;
}


void Actuators::GoLeft() {
    this->TurnLeft();
    this->OneInch();
    this->FollowLine();
}


void Actuators::GoStraight() {
    this->OneInch();
    this->FollowLine();
}


void Actuators::OneInch() {
    struct timeval start, current;
    gettimeofday(&start);
    this->UpdateMotorVelocities(0.1, 0.1);
    double elapsed_time = 0;
    while (elapsed_time < 0.5) {
        gettimeofday(&current, NULL);
        elapsed_time = current.tv_sec - start.tv_sec;
    }
    this->UpdateMotorVelocities(0, 0);
}


void Actuators::GoRight() {
    this->TurnRight();
    this->OneInch();
    this->FollowLine();
}


void Actuators::GoBack() {
    this->TurnBack();
    this->OneInch();
    this->FollowLine();
}


void Actuators::Stop() {
    this->UpdateMotorVelocities(0, 0);
}


void Actuators::FollowLine() {
    // PID controller
    this->UpdateErrorValues();
    
    // Compute PID value 
    float P = this->current_error_value;
    float D = this->current_error_value - this->previous_error_value;
    float pid_value = (this->kKp * P) + (this->kKd * D);

    // Compute velocities after correction
    float velocity_left = this->kNominalMotorVelocity;
    float velocity_right = this->kNominalMotorVelocity;
    velocity_left += pid_value;  
    velocity_right -= pid_value;

    // Apply new velocities
    this->UpdateMotorVelocities(velocity_left, velocity_right);
}


void Actuators::UpdateErrorValues() {
    this->previous_error_value = error_value;
    int* sensors_vector = this->sensors->GetErrorValue();
    this->current_error_value = this->ComputeErrorValue(sensors_vector);
}


void Actuators::TurnLeft() {
    this->Rotate(-this->kPI/2);
}


void Actuators::TurnRight() {
     this->Rotate(this->kPI/2)
}


void Actuators::TurnBack() {
     this->Rotate(-this->KPI);
}


void Actuators::Rotate(float angle) {
    int clock_wise = angle > 0. ? 1 : -1; 

    // Rotate for certain amount of time without looking at sensors
    // Explain why...
    float fraction_of_full_turn = angle / this->kPI * 2;
    float rotation_time = (fraction_of_full_turn * this->TimeFullTurn) / 2;
    struct timeval start, current;
    gettimeofday(&start, NULL);
    double elapsed_time = 0;
    this->UpdateMotorVelocities(clock_wise * this->kNominalMotorVelocity,
        -clock_wise * this->kNominalMotorVelocity);
    while (elapsed_time < rotation_time) {
        gettimeofday(&current, NULL);
        elapsed_time = current.tv_sec - start.tv_sec;
    }

    // 
    this->UpdateErrorValues();
    float nominal_velocity = this->kNominalMotorVelocity;
    float current_velocity = nominal_velocity;
    while(this->current_error_value != 0) {
        int abs_error_value = abs(this->current_error_value);
        if (abs_error_value == 4)
            current_velocity = nominal_velocity * 0.8;
        else if (abs_error_value == 3)
            current_velocity = nominal_velocity * 0.6;
        else if (abs_error_value == 2)
            current_velocity = nominal_velocity * 0.4;
        else if (abs_error_value == 1)
            current_velocity = nominal_velocity * 0.2;

        this->UpdateMotorVelocities(clock_wise * current_velocity,
                -clock_wise * current_velocity);
        
        extApi_sleepMs(10);
        this->UpdateErrorValues();
    }
    this->UpdateMotorVelocities(0,0);
}


float Actuators::GetVelocity() {
    // Returns the norm of the single velocity vector taken for a point-model
    float norm_velocity_left = this->GetLinearVelocityLeftWheel();
    float norm_velocity_right = this->GetLinearVelocityRightWheel();
    float distance_wheel_to_center = this->kInterWheelDistance/2.;
    float angle_center_to_left = atan(velocity_left/distance_wheel_to_center);
    float angle_center_to_right = atan(velocity_right/distance_wheel_to_center);
    float l1 = sqrt(pow(linear_velocity_left, 2) +
                                            pow(a, 2));
    float l2 = sqrt(pow(linear_velocity_right, 2) +
                                            pow(a, 2));
    float norm_velocity_from_center = sqrt(pow(l1, 2) + pow(l2, 2) +
            2*l1*l2*cos(angle_center_to_left + angle_center_to_right));
    return(norm_velocity_from_center);
}


float Actuators::GetLinearVelocityLeftWheel() {
    ;
}


float Actuators::GetLinearVelocityRightWheel() {
    ;
}


void Actuators::UpdateMotorVelocities(float velocity_left,
    float velocity_right) {
    float angular_velocity_left = velocity_left / this->kWheelRadius;
    float angular_velocity_right = velocity_right / this->kWheelRadius;
}








