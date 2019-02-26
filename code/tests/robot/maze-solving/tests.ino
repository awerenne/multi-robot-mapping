/*
    Simple motor manipulation.
*/



//============
void test(byte test_id) {
    switch (test_id) {
        case 1:
            test_1();
            break;
        case 2:
            test_2();
            break;
        case 3:
            test_3();
            break;
    }
}



//============
void test_1() {
    digitalWrite(led_signal, HIGH);  
    coex->calibration();
    digitalWrite(led_signal, LOW);  
    delay(3000);
    Sensors* sensors = coex->getSensors();
    FrequencyState *f_speed_ctrl = new FrequencyState(10);
    FrequencyState *f_dir_ctrl = new FrequencyState(50);
    coex->newLine(6, false);
    unsigned long prev_t = millis();
    float dist = 0, mean = 0;
    float i = 1;
    while (true) {
        if (coex->followLine() == 1) {
            coex->stop();
            coex->sendMsg(String(dist) + "-" + String(mean));
            break;
        }
        if (f_speed_ctrl->isNewState()) {
            float v = sensors->getSpeed();
            float delta_t = (float) (millis()-prev_t);
            dist += v * delta_t / 1000;
            prev_t += delta_t;
        }
        if (f_dir_ctrl->isNewState()) {
            float err = sensors->getError();
            err = err < 0 ? -err : err;
            mean = (i-1)/i*mean + err/i;
            i++;
        }
        delay(5);
    }
    delay(5000);
}


//============
void test_2() {
    ;
}


//============
void test_3() {
    ;
}
