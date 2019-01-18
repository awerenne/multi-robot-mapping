/*
    General description of tests
*/


const byte pin_EN12 = 6;
const byte pin_IN1 = 4;
const byte pin_IN2 = 12;

const byte pin_EN34 = 9;
const byte pin_IN3 = 7;
const byte pin_IN4 = 8;


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
    delay(5000);
    Serial.println("Begin of test...");
}


//============
void loop() {
    // Basic manipulation tests of motors
    
    update_pwm_left(200);
    update_pwm_right(200);
    delay(200000);
    // test_1_1();
    // test_1_2();
    // test_1_3();
    exit(0);
}
























