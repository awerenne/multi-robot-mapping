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
    if (messenger->receiveMessage()) {
        messenger->parseMessage();
        float kp, kd, ki;
        int selection = (int) messenger->getMessage()[0];
        kp = (float) messenger->getMessage()[1];
        kd = (float) messenger->getMessage()[2];
        ki = (float) messenger->getMessage()[3];

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
    if (messenger->receiveMessage()) {
        messenger->parseMessage();
        float kp, kd, ki; 
        instruction = (int) messenger->getMessage()[0];
        kp = (float) messenger->getMessage()[1];
        kd = (float) messenger->getMessage()[2];
        ki = (float) messenger->getMessage()[3];
        pid_line->setParameters(kp, kd, ki); 
    }
}


//============
float speed_control(float target_speed) {
    float error = target_speed - sensors->getSpeed();
    float beta = pid_speed->correction(error);
    if (beta < 0) return 0;
    if (beta > 255) return 255;
    return beta; 
}


//============
float speed_control_left(float target_speed) {
    float error = target_speed - sensors->getSpeedLeft();
    float beta = pid_speed->correction(error);
    if (beta < 0) return 0;
    if (beta > 255) return 255;
    return beta;
}


//============
float speed_control_right(float target_speed) {
    float error = target_speed - sensors->getSpeedRight();
    float beta = pid_speed->correction(error);
    if (beta < 0) return 0;
    if (beta > 255) return 255;
    return beta;
}


//============
float forward_control() {
    float error = sensors->getSpeedRight() - sensors->getSpeedLeft();
    return pid_forward->correction(error);
}


//============
float line_control() {
    float error = sensors->getError();
    error = floorf((error/1250.)*100)/100; 
    return pid_line->correction(error);
}


//============
float turn_control() {
    float error = sensors->getSpeedLeft() + sensors->getSpeedRight();
    return pid_forward->correction(error);
}
