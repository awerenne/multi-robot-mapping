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
float speed_control(float target_speed) {
    float error = target_speed - sensors->getSpeed();
    return pid_speed->update(error);
}


//============
float speed_control_left(float target_speed) {
    float error = target_speed - sensors->getSpeedLeft();
    return pid_speed->update(error);
}


//============
float speed_control_right(float target_speed) {
    float error = target_speed - sensors->getSpeedRight();
    return pid_speed->update(error);
}


//============
float forward_control() {
    float error = sensors->getSpeedRight() - sensors->getSpeedLeft();
    return pid_forward->update(error);
}


//============
float line_control() {
    float error = sensors->getError();;
    return pid_line->update(error);
}


//============
float turn_control() {
    float error = sensors->getSpeedLeft() + sensors->getSpeedRight();
    return pid_forward->update(error);
}
