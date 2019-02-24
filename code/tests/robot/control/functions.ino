/* 
    ..
*/


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
void receive_msg_pid() {
    bool new_msg = messenger->receiveMessage(); 
    if (new_msg == true) {
        float kp, kd, ki;
        int selection = 0;
        messenger->parseMessage();  
        messenger->updateInstruction(selection);
        messenger->updateParameters(kp, kd, ki);

        switch (int(selection)) {
            case 0: pid_speed->setParameters(kp, kd, ki); break;
            case 1: pid_forward->setParameters(kp, kd, ki); break;
            case 2: pid_line->setParameters(kp, kd, ki); break;
            default: break;
        }
    }
}


//============
void receive_msg_line() {
    bool new_msg = messenger->receiveMessage(); 
    if (new_msg == true) {
        float kp, kd, ki;
        messenger->parseMessage();  
        messenger->updateInstruction(instruction);
        messenger->updateParameters(kp, kd, ki);
        pid_line->setParameters(kp, kd, ki); 
    }
}


//============
float speed_control(float target_speed) {
    float error = target_speed - sensors->getSpeed();
    float beta = pid_speed->compute(error);
    if (beta < 0) return 0;
    if (beta > 255) return 255;
    return beta; 
}


//============
float speed_control_left(float target_speed) {
    float error = target_speed - sensors->getSpeedLeft();
    float beta = pid_speed->compute(error);
    if (beta < 0) return 0;
    if (beta > 255) return 255;
    return beta;
}


//============
float speed_control_right(float target_speed) {
    float error = target_speed - sensors->getSpeedRight();
    float beta = pid_speed->compute(error);
    if (beta < 0) return 0;
    if (beta > 255) return 255;
    return beta;
}


//============
float forward_control() {
    float error = sensors->getSpeedRight() - sensors->getSpeedLeft();
    return pid_forward->compute(error);
}


//============
float line_control() {
    float error = sensors->getError();
    Serial.println(error);
    return pid_line->compute(-error);
}


//============
float turn_control() {
    float error = sensors->getSpeedLeft() + sensors->getSpeedRight();
    return pid_forward->compute(error);
}
