#include "Messenger.h"


Messenger::Messenger(char id, SoftwareSerial* port) {
    this->id = id;
    this->port = port;
    while (!this->port) { continue; }  // Wait for connection
    this->port->begin(9600);
}


void Messenger::Send(String msg) {
    String s = String(this->id);
    s += "," + String(this->seq_number);
    s += "," + msg + "/";
    int checksum = 0;
    s += String(checksum) + "/";
    this->port->print(s);
}


String Messenger::Receive() {
    /* Message in the following form: 
         src,seq_number,att_name1:att_value_1, ... 
         ... att_nameN:att_value_N/checksum/
    */
    String s;
    this->port->listen();
    while (this->port->available() > 0) {
        delay(5);  // Small delay to allow input buffer to fill
        char c = this->port->read();  
        if (c == ':') { 
            ; // TODO
        }
        if (c == ',') {
            ; // TODO
        }
        if (c == '/') {
            break; // TODO
        }
        s += c; 
    }
    return s;
}





