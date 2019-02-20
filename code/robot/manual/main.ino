/*
    Test two simple combinations of components (communication & motors, object sensors & motors)
*/


#include <SoftwareSerial.h>
#include <Messenger.h>
#include <Actuators.h>
#include <Sensors.h>


const byte pins_messenger[2] = {10, 11}; // RX, TX
const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  // IN1, IN2, EN12, IN3, IN4, EN34
const byte pins_qta[6] = {0, 1, 2, 3, 4, 5};  // Analog 
const byte pin_sharp = 6;
const byte led_signal = 13;

Messenger* messenger = new Messenger(pins_messenger);
Actuators* actuators = new Actuators(pins_actuators);
const unsigned int parameters_qta[3] = {5, 6, 10};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
Sensors* sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);

byte instruction = -1; // 0 stop, 1 straight, 2 turn left, 3 turn right, 4 turn back


//============
void setup() {
    Serial.begin(9600);
    while(!Serial) continue;
    flicker_led(led_signal, 10, 500);
}


//============
void loop() {
    test_6_1();
    // test_6_2();
    exit(0);
}

//============
void test_6_1() {
    while (true) {
        bool new_msg = messenger->ReceiveMessage();  
        if (new_msg == true) {
            messenger->ParseInstruction();  // received messages are in the following format: <id_sender, seq_number_sender, instruction>
            instruction = messenger->GetInstruction();
        }
    
        switch (instruction) {
            case 0:  // Stop
                actuators->Stop();
                digitalWrite(led_signal, LOW);
                break; 
            case 1:  // Go forward
                actuators->UpdateSpeed(60, 60);
                digitalWrite(led_signal, HIGH);
                break;
            case 2:  // Turn left
                actuators->Turn(false, false);
                digitalWrite(led_signal, HIGH);
                instruction = 0;
                break;
            case 3:  // Turn right
                actuators->Turn(true, false);
                digitalWrite(led_signal, HIGH);
                instruction = 0;
                break;
            case 4:  // Full turn
                actuators->Turn(false, true);
                digitalWrite(led_signal, HIGH);
                instruction = 0;
                break;
        }
        
        delay(250);
    }
}