#include "Coex.h"


//============
Coex::Coex() {
    ;
}


//============
byte Coex::followLine(const float& speed) {
    ;
}


//============
byte Coex::forward(const float& speed) {
    ;
}


//============
void Coex::oneInch(const float& speed) {
    ;  // Use forward to be straight
}


//============
void Coex::turn90(const bool& clockwise, const float& speed) {
    ;
}


//============
void Coex::turn180(const bool& clockwise, const float& speed) {
    ;
}


//============
void Coex::turnLeft(const float& speed) {
    ;
}


//============
void Coex::turnRight(const float& speed) {
    ;
}


//============
void Coex::uturn(const float& speed) {
    ;
}


//============
bool Coex::isObstacle() {
    ;
}


//============
bool Coex::isIntersection() {
    ;
}


//============
byte Coex::typeIntersection() {
    ;
}



// //============
// void Actuators::UpdateSpeed(const float& speed_left, const float& speed_right) {
//     int pwm_left = speed_left;
//     int pwm_right;
//     if (speed_right < 0)
//         pwm_right = speed_right - 12;  // TODO: change to regression functions
//     else 
//         pwm_right = speed_right + 12;  // TODO: change to regression functions
//     this->updatePWM(pwm_left, pwm_right);
// }


// //============
// void Actuators::Accelerate() {
//     this->target_speed += 5;
// }


// //============
// void Actuators::Turn(const bool& clockwise, const bool& full, Sensors* sensors=NULL) {
//     int n_quarters = full ? 2 : 1;
//     float target_speed = this->target_speed * 0.93;
//     for (int i = 0; i < n_quarters; ++i) {
//         if (clockwise)
//             this->UpdateSpeed(target_speed, -target_speed);
//         else
//             this->UpdateSpeed(-target_speed, target_speed);
//         delay(450);  // Time to do slightly less than half turn
//     }
//     if (sensors)
//         this->TurnToLine(clockwise, sensors);
//     this->Stop();
//     delay(200);
// }


// //============
// // Continue to turn until line is in centered (explain more)
// void Actuators::TurnToLine(const bool& clockwise, Sensors* sensors) {
//     float target_speed = this->target_speed * 0.895;
//     if (clockwise)
//         this->UpdateSpeed(target_speed, -target_speed);
//     else
//         this->UpdateSpeed(-target_speed, target_speed);
//     do {
//         delay(5);
//         sensors->QTRARead();
//     } while (!sensors->IsAligned(clockwise));
// }

// //============
// // PID controll and new line trick (explain more)
// void Actuators::FollowLine(const int& error) { 
//     this->accumulated_error += error;
//     if (this->new_line) {
//         this->pid_value = this->kp * error;
//         this->new_line = false;
//         this->start_acceleration = millis();
//     }
//     else
//         this->pid_value = (this->kp * error) + (this->kd *
//             (error - this->previous_error)) + (this->ki * this->accumulated_error);
//     this->previous_error = error;

//     float target_speed = this->target_speed;
//     if ((millis() - this->start_acceleration) < 800)
//         target_speed *= 0.87;

//     float speed_left = target_speed - pid_value;
//     float speed_right = target_speed + pid_value;
//     this->UpdateSpeed(speed_left, speed_right); 
// }


// //============
// void Actuators::OneInch() {
//     float target_speed = this->target_speed * 0.93;
//     this->UpdateSpeed(target_speed, target_speed);
//     delay(400);
//     this->Stop();
//     this->new_line = true;
// }

// //============
// byte Sensors::TypeIntersection(Actuators* actuators) {  
//     bool is_road_left = this->IsRoadLeft();
//     bool is_road_here = this->IsRoadHere();
//     bool is_road_right = this->IsRoadRight();

//     actuators->OneInch();
//     delay(500);

//     this->QTRARead();
//     bool is_road_front = this->IsRoadHere();

//     actuators->OneInch();
//     delay(1000);

//     if (is_road_left && is_road_front && is_road_right)
//         return 0;
//     else if (is_road_left && is_road_front && !is_road_right)
//         return 1;
//     else if (!is_road_left && is_road_front && is_road_right)
//         return 2;
//     else if (is_road_left && !is_road_front && is_road_right)
//         return 3;
//     else if (is_road_left && !is_road_front && !is_road_right)
//         return 4;
//     else if (!is_road_left && !is_road_front && is_road_right)
//         return 5;
//     return 6;
// }


// //============
// bool Sensors::IsIntersection(Actuators* actuators) { 
//     this->QTRARead();
//     bool is_road_left = this->IsRoadLeft();
//     bool is_road_here = this->IsRoadHere();
//     bool is_road_right = this->IsRoadRight();
//     if (is_road_left || is_road_right || !is_road_here) {
//         actuators->Stop();
//         delay(1000);
//         return true;
//     } 
//     return false;
// }


// //============
// void Messenger::ParsePIDParameters() {      // split the data into its parts
//     strcpy(this->tempChars, this->receivedChars);

//     char * strtokIndx; // this is used by strtok() as an index

//     strtokIndx = strtok(this->tempChars,"/");      // get the first part - the string
//     this->instruction = atoi(strtokIndx);     // convert this part to an integer
 
//     strtokIndx = strtok(NULL, "/"); // this continues where the previous call left off
//     this->kp = atof(strtokIndx);     // convert this part to an integer

//     strtokIndx = strtok(NULL, "/"); // this continues where the previous call left off
//     this->kd = atof(strtokIndx);     // convert this part to an integer

//     strtokIndx = strtok(NULL, "/"); // this continues where the previous call left off
//     this->ki = atof(strtokIndx);     // convert this part to an integer

//     this->newData = false;
// }


// //============
// int Messenger::GetInstruction() {
//     return this->instruction;
// }


// //============
// float Messenger::GetKp() {
//     return this->kp;
// }


// //============
// float Messenger::GetKd() {
//     return this->kd;
// }


// //============
// float Messenger::GetKi() {
//     return this->ki;
// }





