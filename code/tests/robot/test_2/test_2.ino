/*
    General description of tests
*/


const byte pin_sharp = 6;  // Analog pin 6  
const byte led_sharp = 7;  // Analog pin 6  
const int f = 50;  // Frequency
const int T = 30000;  // Total test time (in milliseconds)

// Test performed with exteranl lights on, 
// external lights can sometimes influence certain sensor measures
const bool light_on = true;  


//============
void setup() {
    pinMode(pin_sharp, INPUT);
    pinMode(led_sharp, OUTPUT);
    Serial.begin(9600);
    while (!Serial) continue;
    delay(5000);
    Serial.println("Begin test...");
}


//============
void loop() {
    test_2_1();
    exit(0);
}













