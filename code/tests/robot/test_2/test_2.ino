/*
    Tests on sharp sensor.
*/


const byte pin_sharp = 6;  // Analog pin 6  
const byte led_sharp = 7;  // Analog pin 7 
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
}


//============
void loop() {
    test_2_1();
    exit(0);
}


//============
void flicker_led(byte led, unsigned int n, unsigned int delay_) {
    for (int i = 0; i < n; i++) {
        digitalWrite(led, LOW);
        delay(delay_);
        digitalWrite(led, HIGH);
        delay(delay_);
    }
}










