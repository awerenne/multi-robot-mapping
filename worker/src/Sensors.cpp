

#include "../include/Sensors.h"


Sensors::Sensors() : kNumberOfSensors(7) {
    ;
}


int* Sensors::GetVector() {
    int* sensors_vector = new int[this->kNumberOfSensors];
    return sensors_vector;
}


std::string Sensors::GetSituation() {
    int* sensors_vector = this->GetVector();
    if (this->Compare(sensors_vector, "x00100x"))
        return "normal";
    else if (false)  // Chack unvalid inputs (bad sequence ... but no line -> intersection!!!!)
        return "not-valid";
    else 
        return "intersection";
}


char Sensors::GetTypeIntersection(int* sensors_vector,
        int* sensors_vector_one_inch) {
    bool left_path = this->Compare(sensors_vector, "1xxxxxx");
    bool in_front_path = false;
    if (this->Compare(sensors_vector_one_inch, "x1xxxxx") || 
        this->Compare(sensors_vector_one_inch, "xx1xxxx") ||
        this->Compare(sensors_vector_one_inch, "xxx1xxx") ||
        this->Compare(sensors_vector_one_inch, "xxxx1xx") ||
        this->Compare(sensors_vector_one_inch, "xxxxx1x"))
        in_front_path = true;
    bool right_path = this->Compare(sensors_vector, "xxxxxx1");

    if (left_path && in_front_path && right_path)
        return 'A';
    else if (left_path && in_front_path && !right_path)
        return 'B';
    else if (!left_path && in_front_path && right_path)
        return 'C';
    else if (left_path && !in_front_path && right_path)
        return 'D';
    else if (left_path && !in_front_path && !right_path)
        return 'E';
    else if (left_path && !in_front_path && !right_path)
        return 'F';
    else if (!left_path && !in_front_path && !right_path)
        return 'G';
    return 'U';  // Return undetermined intersection
}


int Sensors::GetErrorValue() {
    int* sensors_vector = this->GetVector();

    if (this->Compare(sensors_vector, "x00001x"))
        return(4);

    else if (this->Compare(sensors_vector, "x00011x"))
        return(3);

    else if (this->Compare(sensors_vector, "x0001xx"))
        return(2);

    else if (this->Compare(sensors_vector, "x00110x"))
        return(1);

    else if (this->Compare(sensors_vector, "x00100x"))
        return(0);

    else if (this->Compare(sensors_vector, "x01100x"))
        return(-1);

    else if (this->Compare(sensors_vector, "x01000x"))
        return(-2);

    else if (this->Compare(sensors_vector, "x11000x"))
        return(-3);

    else if (this->Compare(sensors_vector, "x10000x"))
        return(-4);
    
    return(0);  
}


bool Sensors::Compare(int* sensors_vector, std::string condition) {
    if (condition.size() != this->kNumberOfSensors)
        return false;

    for (int i = 0; i < this->kNumberOfSensors; ++i) {
        if (condition[i] == 1) {
            if (sensors_vector[i] == 0)
                return false;
        }

        else if (condition[i] == 0) {
            if (sensors_vector[i] == 1)
                return false;
        }
    }

    return true;
}





