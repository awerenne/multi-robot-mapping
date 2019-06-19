/*
    Tests on sharp sensor.
*/


const byte pin_sharp = A6;  // Analog pin 6  
const byte led_sharp = A7;  // Analog pin 7 
const byte led_signal = 13;

const int f = 50;  // Frequency
const int T = 30000;  // Total test time (in milliseconds)
const bool light_on = true;  


//============
void setup() {
    pinMode(pin_sharp, INPUT);
    pinMode(led_sharp, OUTPUT);
    Serial.begin(9600);
    while (!Serial) continue;
    flicker_led(led_signal, 10, 500);
    digitalWrite(led_signal, HIGH);
}


//============
void loop() {
    test(1);
    exit(0);
}
