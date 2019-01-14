/*
    General description of tests
*/


#include <SoftwareSerial.h>


SoftwareSerial Bluetooth(10, 11);  // RX, TX
const unsigned int T = 100000;  // Test time (in milliseconds)
unsigned long init_time;  // Initial moment test begins (stop test when time = init_time + T)

unsigned int id_master = 0;
unsigned long seq_number_master = 0;
unsigned int id_slave = 0;
unsigned long seq_number_slave = 0;

int instruction = -1;
int mode = -1;
float kp = 0;
float kd = 0; 
float ki = 0;
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];  // temporary array for use when parsing
boolean newData = false;


//============
void setup() {
    Serial.begin(9600);
    while(!Serial) continue;
    delay(5000);
    Serial.println("Begin test...");
    Serial.println("Waiting for connection...");
    Bluetooth.begin(9600);
    while(!Bluetooth) continue;
}


//============
void loop() {
    test_4_1();
    // test_4_2();
    exit(0);
}























