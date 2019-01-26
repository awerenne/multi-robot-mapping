/* 
    Most of this code was borrowed from 
        http://forum.arduino.cc/index.php?topic=396450
    and adapted to my needs. Many thanks to the author!
*/


#include "Messenger.h"


//============
Messenger::Messenger(const byte* pins) {
    this->blt = new SoftwareSerial(pins[0], pins[1]);  // RX, TX
    while (!this->blt) continue;  // Wait for connection
    this->blt->begin(9600);
}


//============
bool Messenger::ReceiveMessage() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (this->blt->available() > 0 && this->newData == false) {
        rc = this->blt->read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                this->receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                this->receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                this->newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
    return this->newData;
}


//============
void Messenger::ParseInstruction() {      // split the data into its parts
    strcpy(this->tempChars, this->receivedChars);

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(this->tempChars,"/");      // get the first part - the string
    this->id_master = atoi(strtokIndx);     // convert this part to an integer
 
    strtokIndx = strtok(NULL, "/"); // this continues where the previous call left off
    this->seq_number_master = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, "/");
    this->instruction = atoi(strtokIndx);     // convert this part to an integer

    this->newData = false;
}


//============
void Messenger::ParsePIDParameters() {      // split the data into its parts
    strcpy(this->tempChars, this->receivedChars);

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(this->tempChars,"/");      // get the first part - the string
    this->instruction = atoi(strtokIndx);     // convert this part to an integer
 
    strtokIndx = strtok(NULL, "/"); // this continues where the previous call left off
    this->kp = atof(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, "/"); // this continues where the previous call left off
    this->kd = atof(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, "/"); // this continues where the previous call left off
    this->ki = atof(strtokIndx);     // convert this part to an integer

    this->newData = false;
}


//============
void Messenger::SendMessage(const String& msg) {
    String datagram = "<" + String(this->id_slave) + "/" +
        String(this->seq_number_slave) + "/" + msg + ">\n";
    this->blt->println(datagram);
    this->seq_number_slave += 1;
}


//============
int Messenger::GetInstruction() {
    return this->instruction;
}


//============
float Messenger::GetKp() {
    return this->kp;
}


//============
float Messenger::GetKd() {
    return this->kd;
}


//============
float Messenger::GetKi() {
    return this->ki;
}













