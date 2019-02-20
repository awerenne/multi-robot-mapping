/* 
    Code inspired from 
        http://forum.arduino.cc/index.php?topic=396450
    and adapted to my needs. Many thanks to the author!
*/


#include "Messenger.h"


//============
Messenger::Messenger(const byte* pins, int baud_rate, int parse_option=0) {
    this->initBluetooth(pins, baud_rate);
    this->resetCommunication();
    this->parse_option = parse_option;
    this->id_self = 1;
    this->id_master = 0;
    this->start_marker = '<';
    this->end_marker = '>';
    this->received_chars = (char*) malloc(this->num_chars * sizeof(char));
    this->temp_chars = (char*) malloc(this->num_chars * sizeof(char));
}


//============
void Messenger::resetCommunication() {
    this->seq_number_self = 0;
    this->seq_number_master = 0;
    this->new_data = false;
    this->instruction = 0;
    this->kp = 0.05;
    this->kd = 0;
    this->ki = 0;
}


//============
void Messenger::initBluetooth(const byte* pins, int baud_rate) {
    this->bluetooth = new SoftwareSerial(pins[0], pins[1]);  
    while (!this->bluetooth) continue;  
    this->bluetooth->begin(baud_rate);
}


//============
bool Messenger::receiveMessage() {
    static boolean reception_in_progress = false;
    static byte index = 0;
    char received_char;

    while (this->bluetooth->available() > 0 && this->new_data == false) {
        received_char = this->bluetooth->read();

        if (reception_in_progress == true) {
            if (received_char != this->end_marker) {
                this->received_chars[index] = received_char;
                index++;
                if (index >= this->num_chars) index = this->num_chars - 1;
            }
            else {
                this->received_chars[index] = '\0';  // terminate 
                reception_in_progress = false;
                index = 0;
                this->new_data = true;
            }
        }

        else if (received_char == this->start_marker) 
            reception_in_progress = true;
    }
    return this->new_data;
}


//============
void Messenger::sendMessage(const String& information) {
    String msg = String(this->start_marker); 
    msg += String(this->id_self) + "/";
    msg += String(this->seq_number_self) + "/";
    msg += String(information) + String(this->end_marker) + "\n";
    this->bluetooth->println(msg);
    this->seq_number_self += 1;
}


//============
void Messenger::parseMessage() {
    switch (this->parse_option) {
        case 0: this->parseNormal(); break;
        case 1: this->parsePID(); break;
        default: this->parseNormal(); break;
    }
}


//============
void Messenger::parseNormal() {
    strcpy(this->temp_chars, this->received_chars);
    char* ptr; 

    ptr = strtok(this->temp_chars, "/");     
    this->id_master = atoi(ptr); 

    ptr = strtok(NULL, "/"); 
    this->seq_number_master = atoi(ptr); 

    ptr = strtok(NULL, "/");
    this->instruction = atoi(ptr);     
    this->new_data = false;
}


//============
void Messenger::parsePID() {
    strcpy(this->temp_chars, this->received_chars);
    char* ptr; 

    ptr = strtok(this->temp_chars, "/");     
    this->instruction = atoi(ptr);     

    ptr = strtok(NULL, "/"); 
    this->kp = atof(ptr);   

    ptr = strtok(NULL, "/");
    this->kd = atof(ptr);

    ptr = strtok(NULL, "/");
    this->ki = atof(ptr);       
    this->new_data = false;
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














