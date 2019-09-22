#ifndef MESSENGER_H_
#define MESSENGER_H_

#include <Arduino.h>
#include <SoftwareSerial.h>


class Messenger {
    public:
        Messenger(const byte* pins, int baud_rate, int id_=1);
        void sendMessage(const String& information, int first=1);
        bool receiveMessage();
        int parseMessage();
        float* getMessage();
    
    private:
        SoftwareSerial* bluetooth;
        int id_self, seq_number, parse_option;
        int id_master;
        char start_marker, end_marker;
        const byte num_chars = 64;
        char* received_chars;
        char* temp_chars;
        String message;       
        bool new_data = false;
        float data[5]; 

        void resetCommunication();
        void initBluetooth(const byte* pins, int baud_rate);
        // TODO: checksum, parity etc

};



#endif  // MESSENGER_H_




