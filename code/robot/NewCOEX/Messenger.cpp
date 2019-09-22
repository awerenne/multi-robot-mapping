#include "Messenger.h"


//============
Messenger::Messenger(const byte* pins, int baud_rate, int id_=1) {
    initBluetooth(pins, baud_rate);
    resetCommunication();
    id_self = id_;
    id_master = 0;
    start_marker = '<';
    end_marker = '>';
    received_chars = (char*) malloc(num_chars * sizeof(char));
    temp_chars = (char*) malloc(num_chars * sizeof(char));
}


//============
void Messenger::resetCommunication() {
    seq_number = 0;
    new_data = false;
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
                received_chars[index] = received_char;
                if (index >= num_chars) index = num_chars - 1;
                index++;
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
    return new_data;
}


//============
void Messenger::sendMessage(const String& information, int first=1) {
    //if (first)
    seq_number += 1;
    String msg = String(start_marker); 
    msg += String(id_self) + "/";
    msg += String(seq_number) + "/";
    msg += String(information) + String(end_marker) + "\n";
    //delay(1000);
    for (int i = 0; i < 1; i++) {
        bluetooth->println(msg);
        //delay(200);
    }
}


//============
int Messenger::parseMessage() {
    strcpy(temp_chars, received_chars);
    char* ptr; 
    ptr = strtok(temp_chars, "/");     
    int dest = atoi(ptr); 
    if (dest != id_self) 
        return -1;
    ptr = strtok(NULL, "/"); 
    int new_seq = atoi(ptr); 
    // if (seq_number > new_seq) 
    //     return -1;
    seq_number = new_seq;
    for (int i = 0; i < 5; i++) {
        ptr = strtok(NULL, "/"); 
        data[i] = atof(ptr);   
    }      
    new_data = false;
    return 0;
} 


//============
float* Messenger::getMessage() {
    return data;
} 












