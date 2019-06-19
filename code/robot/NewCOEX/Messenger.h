#ifndef MESSENGER_H_
#define MESSENGER_H_

#include <Arduino.h>
#include <SoftwareSerial.h>


class Messenger {
    public:
        Messenger(const byte* pins, int baud_rate);
        void sendMessage(const String& information);
        bool receiveMessage();
        void parseMessage();
        float* getMessage();
    
    private:
        SoftwareSerial* bluetooth;
        int id_self, seq_number_self, parse_option;
        int id_master, seq_number_master;
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




