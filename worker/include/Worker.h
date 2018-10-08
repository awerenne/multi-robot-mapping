//
// Created by aurelien-werenne on 19/09/2018.
//

#ifndef WORKER_H_
#define WORKER_H_


#include <string>
#include <unistd.h>


#include "Sensors.h"
#include "Actuators.h"


class Worker {
 public:
    Worker(const int& id);
    void Run();
    int GetId();
    

 private:
    int id;
    float distance;
    bool run;
    unsigned int time_step;  // In milliseconds
    Sensors* sensors;
    Actuators* actuators;

    std::string ComputeDirection(char type_intersection);
    float ComputeDistance(float velocity);
    void Send(int receiver_id, std::string type_message,
        float content_message);


};

#endif  // WORKER_H_