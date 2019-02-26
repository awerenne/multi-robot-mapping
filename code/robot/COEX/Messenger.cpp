#include "Messenger.h"


//============
Messenger::Messenger(const byte* pins, int baud_rate, int parse_option=0) {
    initBluetooth(pins, baud_rate);
    resetCommunication();
    this->parse_option = parse_option;
    id_self = 1;
    id_master = 0;
    start_marker = '<';
    end_marker = '>';
    received_chars = (char*) malloc(num_chars * sizeof(char));
    temp_chars = (char*) malloc(num_chars * sizeof(char));
}


//============
void Messenger::resetCommunication() {
    seq_number_self = 0;
    seq_number_master = 0;
    new_data = false;
    instruction = 0;
    kp = 0;
    kd = 0;
    ki = 0;
}


//============
void Messenger::initBluetooth(const byte* pins, int baud_rate) {
    bluetooth = new SoftwareSerial(pins[0], pins[1]);  
    while (!bluetooth) continue;  
    bluetooth->begin(baud_rate);
}


//============
bool Messenger::receiveMessage() {
    static boolean reception_in_progress = false;
    static byte index = 0;
    char received_char;

    while (bluetooth->available() > 0 && new_data == false) {
        received_char = bluetooth->read();
        if (reception_in_progress == true) {
            if (received_char != end_marker) {
                received_chars[index++] = received_char;
                if (index >= num_chars) index = num_chars - 1;
            }
            else {
                received_chars[index] = '\0';  // terminate 
                reception_in_progress = false;
                index = 0;
                new_data = true;
            }
        }
        else if (received_char == start_marker) reception_in_progress = true;
    }
    return this->new_data;
}


//============
void Messenger::sendMessage(const String& information) {
    String msg = String(start_marker); 
    msg += String(id_self) + "/";
    msg += String(seq_number_self) + "/";
    msg += String(information) + String(end_marker) + "\n";
    bluetooth->println(msg);
    seq_number_self += 1;
}


//============
void Messenger::parseMessage() {
    switch (parse_option) {
        case 0: parseNormal(); break;
        case 1: parsePID(); break;
        default: parseNormal(); break;
    }
}


//============
void Messenger::parseNormal() {
    strcpy(temp_chars, received_chars);
    char* ptr; 

    ptr = strtok(temp_chars, "/");     
    id_master = atoi(ptr); 

    ptr = strtok(NULL, "/"); 
    seq_number_master = atoi(ptr); 

    ptr = strtok(NULL, "/");
    instruction = atoi(ptr);     
    new_data = false;
}


//============
void Messenger::parsePID() {
    strcpy(temp_chars, received_chars);
    char* ptr; 

    ptr = strtok(temp_chars, "/");     
    instruction = atoi(ptr);     

    ptr = strtok(NULL, "/"); 
    kp = atof(ptr);   

    ptr = strtok(NULL, "/");
    kd = atof(ptr);

    ptr = strtok(NULL, "/");
    ki = atof(ptr);       
    new_data = false;
}


//============
void Messenger::updateInstruction(int& instruction) {
    instruction = this->instruction;
}
   

//============
void Messenger::updateParameters(float& kp, float& kd, float& ki) {
    kp = this->kp;
    kd = this->kd;
    ki = this->ki;
}














