//
// Created by aurelien-werenne on 19/09/2018.
//

#ifndef SENSORS_H_
#define SENSORS_H_


#include <string>


class Sensors {
 public:
    Sensors();
    int* GetVector();
    std::string GetSituation();
    char GetTypeIntersection(int* sensors_vector, int* sensors_vector_one_inch);
    int GetErrorValue();
    

 private:
    const unsigned int kNumberOfSensors;
    bool Compare(int* sensors_vector, std::string condition);


};

#endif  // SENSORS_H_