

#include "../include/Worker.h"


Worker::Worker(const int& id) {
    this->id = id;
    this->distance = 0;
    this->run = true;
    this->sensors = new Sensors();
    this->actuators = new Actuators();
    this->time_step = 2;  // In milliseconds
}


void Worker::Run() {
    while (this->run) {
        this->distance++;
        std::string situation = this->sensors->GetSituation()

        if (situation == "normal") 
            this->actuators->GoStraight();

        else if (situation == "not-valid") {
            this->Send(0, "info-map-distance", this->distance);
            this->distance = 0;
            this->actuators->Stop();
            this->run = false;
        }

        else if (situation == "intersection") {
            this->Send(0, "info-map-distance", this->distance);
            this->distance = 0;

            // Read sensors to detemine type of encountered intersection 
            this->actuators->Stop();
            int* sensors_vector = this->sensors->GetVector();
            this->actuators->OneInch();
            int* sensors_vector_one_inch = this->sensors->GetVector();

            // Determine type of intersection
            char type_intersection = this->sensors->GetTypeIntersection(
                sensors_vector, sensors_vector_one_inch);

            // Determine direction to take
            std::string new_direction = this->ComputeDirection(type_intersection);

            // Take direction
            if (new_direction == "left") 
                this->actuators->GoLeft();
            else if (new_direction == "straight")
                this->actuators->GoStraight();
            else if (new_direction == "right")
                this->actuators->GoRight();
            else if (new_direction == "uturn")
                this->actuators->GoBack();
            else {
                this->actuators->Stop();
                this->run = false;
            }
        }

        usleep(this->time_step);

        // Compute traveled distance
        float applied_velocity = this->actuators->GetVelocity();
        float traveled_distance = this->ComputeDistance(applied_velocity);
        this->distance += applied_distance;
    }
}


int Worker::GetId() {
    return this->id;
}


std::string Worker::ComputeDirection(char type_intersection) {
    if (type_intersection == 'A')
        return "left"
    else if (type_intersection == 'B')
        return "left"
    else if (type_intersection == 'C')
        return "straight"
    else if (type_intersection == 'D')
        return "left"
    else if (type_intersection == 'E')
        return "left"
    else if (type_intersection == 'F')
        return "right"
    else if (type_intersection == 'G')
        return "uturn"
    return "stop"
} 


float Worker::ComputeDistance(float velocity) {
    float time_step_in_seconds = ((float) this->time_step) / 1000.;
    return velocity * time_step_in_seconds;  // In cm
}


void Worker::Send(int receiver_id, std::string type_message,
        float content_message) {
    std::cout << "Receiver ID: " << receiver_id << std::endl;
    std::cout << "Type Message: " << type_message << std::endl;
    std::cout << content_message << std::endl;
}









