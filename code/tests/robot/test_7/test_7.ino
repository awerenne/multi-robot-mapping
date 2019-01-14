/*
    Test PID controller to follow a simple line forming an arc.
*/


#include <Messenger.h>
#include <Actuators.h>
#include <Sensors.h>


const byte pins_bluetooth[2] = {10, 11};  // RX, TX
Messenger* messenger = new Messenger(pins_bluetooth);

const byte pins_actuators[6] = {4,12,6,7,8,9};  // IN1, IN2, EN12, IN3, IN4, EN34
Actuators* actuators = new Actuators(pins_actuators);

const byte pins_qta[6] = {0, 1, 2, 3, 4, 5};  // Analog --> Change to 14-21 range
const unsigned int parameters_qta[3] = {5, 6, 10};  // Emitter pin (digital PWM), number of sensors, number of samples to average per sensor reading
const byte pin_sharp = 6;
const byte pins_encoder[2] = {3, 2};
Sensors* sensors = new Sensors(pins_qta, parameters_qta, pin_sharp);

const byte led_running = 13;  // Used to signal when robot is calibrating IR sensors, and when the robot is running
const float target_speed = 65.;  // TODO: to change to speed and not pwm

int error = sensors->GetError(), previous_error = sensors->GetError(), accumulated_error = 0;
float kp = 0, kd = 0, ki = 0, pid_value = 0;
byte instruction = -1; // 0 stop, 1 straight, 2 turn left, 3 turn right, 4 turn back


//============
void setup() {
    Serial.begin(9600);
    while(!Serial) continue; // Wait for init
    delay(5000);
    Serial.println("Begin test...");

    flicker_led(led_running, 10, 200);
    sensors->Calibrate();
    digitalWrite(led_running, LOW);   
}


//============
void loop() {
    bool new_msg = messenger->ReceiveMessage();  
    if (new_msg == true) {
        messenger->ParsePIDParameters();  // received messages are in the following format: <instruction, kp, kd, ki>
        instruction = messenger->GetInstruction();
        kp = messenger->GetKp();
        kd = messenger->GetKd();
        ki = messenger->GetKi();
    }

    switch (instruction) {
        case 0:  // Stop
            actuators->Stop();
            digitalWrite(led_running, LOW);
            break; 
            
        case 1:  // Go forward
            sensors->QTRARead();
            error = sensors->GetError();
            accumulated_error += error;
            pid_value = (kp * error) + (kd * (error - previous_error)) + (ki * accumulated_error);
            previous_error = error;
            float speed_left = target_speed - pid_value;
            float speed_right = target_speed + pid_value;
            actuators->UpdateSpeed(speed_left, speed_right);
            digitalWrite(led_running, HIGH);
            break;
            
        case 2:  // Turn left
            actuators->Turn(false, false);
            instruction = 0;
            digitalWrite(led_running, LOW);
            break;
            
        case 3:  // Turn right
            actuators->Turn(true, false);
            instruction = 0;
            digitalWrite(led_running, LOW);
            break;
            
        case 4:  // Full turn
            actuators->Turn(false, true);
            instruction = 0;
            digitalWrite(led_running, LOW);
            break;
    }

    if (sensors->IsObstacle()) {
        actuators->Stop();
        instruction = 0;
    }
    
    delay(20);
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











