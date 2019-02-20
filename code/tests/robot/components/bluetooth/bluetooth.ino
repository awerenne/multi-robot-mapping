/*
    General description of tests
*/


#include <SoftwareSerial.h>


const byte led_signal = 13;

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
    Bluetooth.begin(9600);
    while(!Bluetooth) continue;
    flicker_led(led_signal, 10, 500);
    digitalWrite(led_signal, HIGH);
}


//============
void loop() {
    test(1);
    exit(0);
}






















