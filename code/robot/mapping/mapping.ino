
#include <Coex.h>

const byte pin_interrupt_left = 3;
const byte pin_interrupt_right = 2;
const byte pins_messenger[2] = {10, 11}; // RX, TX
const byte pins_actuators[6] = {4, 12, 6, 7, 8, 9};  // IN1, IN2, EN12, IN3, IN4, EN34
const byte pins_qta[6] = {A0, A1, A2, A3, A4, A5};  // Analog 
const byte pin_sharp = A6;
const byte led_signal = 13;
const int baud_rate = 9600;
const unsigned int pin_emitter = 5, n_sensors = 6, n_samples = 4;
const unsigned int parameters_qta[3] = {pin_emitter, n_sensors, n_samples}; 
Coex* coex = new Coex(pins_messenger, pins_actuators, pins_qta, pin_sharp,
            baud_rate, parameters_qta, 2);


//============
void setup() {
    pinMode(pin_interrupt_left, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_left), update_counter_left, CHANGE);
    pinMode(pin_interrupt_right, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt_right), update_counter_right, CHANGE);

    Serial.begin(9600);
    while(!Serial) continue;
    flicker_led(led_signal, 10, 200);
    digitalWrite(led_signal, LOW);     
}


//============
void loop() {   
    digitalWrite(led_signal, HIGH);  
    //coex->automaticCalibration();
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(1000);
    float ret = 0, x = 8.5;
    while (true) {
        if(coex->availableMsg() && coex->readMsg() == 0)
            break;
    }
    int instruction = coex->getMsgInstruction();
    bool stopped = true;
    if (instruction != 1) exit(0);
    coex->newLine(8.5, true);
    while (true) {
        ret = coex->followLine();
        if (ret == -1) {
            coex->sendMsg(String(-1) + ";" + String(x));
            flicker_led(led_signal, 10, 1000);
            break;
        }
        if (ret == -2) {
            byte type_intersection = coex->typeIntersection();
            coex->sendMsg(String(type_intersection) + ";" + String(x));
            int instruction;
            //unsigned long init_time = millis();
            while (true) {
//                if (millis() > init_time + 3500) {
//                    coex->sendMsg(String(type_intersection) + ";" + String(-1));
//                    init_time = millis();
//                }
                if(coex->availableMsg() && coex->readMsg() == 0) {
                    instruction = coex->getMsgInstruction();
                    if (instruction != 0)
                        break;
                    else
                        coex->stop();
                }
            }
            
            switch (instruction) {
                case 0: coex->stop();
                        // exit(0);
                        break;
                case 2: coex->turnAlign(6.5, 0, coex->typeIntersection());
                        stopped = false;
                        break;
                case 3: coex->turnAlign(7.5, 1, coex->typeIntersection());
                        stopped = false;
                        break;
                case 4: coex->turnAlign(6, 2, coex->typeIntersection());
                        stopped = false;
                        break;
            }
            coex->newLine(8.5,true);
            x = -ret;
            x += 8.5;
        }
        x += ret;
    }
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


//============
void update_counter_left() { coex->getSensors()->incrementLeft(); }
void update_counter_right() { coex->getSensors()->incrementRight(); }
