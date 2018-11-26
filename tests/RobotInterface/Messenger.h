#ifndef MESSENGER_H_
#define MESSENGER_H_

#include <Arduino.h>
#include <SoftwareSerial.h>


class Messenger {
 public:
    Messenger(char id, SoftwareSerial* port);
    void Send(String msg);
    String Receive();
    
 private:
    char id;
    SoftwareSerial* port;
    unsigned seq_number = 0;
    
    // Checksum
    // Sequence number
    // ...

};

#endif  // MESSENGER_H_

