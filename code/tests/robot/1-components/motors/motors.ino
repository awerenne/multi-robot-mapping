/*
    Basic manipulations of motors.
*/

const byte pin_EN12 = 6;
const byte pin_IN1 = 4;
const byte pin_IN2 = 12;

const byte pin_EN34 = 9;
const byte pin_IN3 = 7;
const byte pin_IN4 = 8;

const byte led_signal = 13;


//============
void setup() {
    pinMode(pin_EN12, OUTPUT);
    pinMode(pin_IN1, OUTPUT);
    pinMode(pin_IN2, OUTPUT);

    pinMode(pin_EN34, OUTPUT);
    pinMode(pin_IN3, OUTPUT);
    pinMode(pin_IN4, OUTPUT);

    digitalWrite(pin_IN1, HIGH);
    digitalWrite(pin_IN2, HIGH);
    digitalWrite(pin_IN3, HIGH);
    digitalWrite(pin_IN4, HIGH);

    Serial.begin(9600);
    while (!Serial) continue;
    flicker_led(led_signal, 10, 500);
    digitalWrite(led_signal, HIGH);
}

//============
void loop() {
    test(3);
    exit(0);
}
