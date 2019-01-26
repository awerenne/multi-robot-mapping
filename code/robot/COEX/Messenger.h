#ifndef MESSENGER_H_
#define MESSENGER_H_


#include "Arduino.h"
#include "SoftwareSerial.h"


class Messenger {
 
 public:
    Messenger(const byte* pins);
    void SendMessage(const String& msg);
    void ParsePIDParameters();
    void ParseInstruction();
    bool ReceiveMessage();
    char* GetMessage();
    int GetInstruction();
    float GetKp();
    float GetKd();
    float GetKi();
    
 private:
    SoftwareSerial* blt;
    int id_slave = 1;
    int seq_number_slave = 0;
    int id_master = 0;
    int seq_number_master = 0;

    const byte numChars = 32;
    char receivedChars[32];
    char tempChars[32];        // temporary array for use when parsing

    int instruction = -1;
    float kp = 0, kd = 0, ki = 0;
    boolean newData = false;

    // Checksum, parity etc
};

#endif  // MESSENGER_H_
