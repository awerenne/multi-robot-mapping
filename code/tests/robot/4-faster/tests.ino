/*
    ...
*/


//============
void test(byte test_id) {
    switch (test_id) {
        case 1: test_1(); break;
        case 2: test_2(); break;
        case 3: test_3(); break;
    }
}



//============
void test_1() {
    flicker_led(led_signal, 10, 200);
    digitalWrite(led_signal, LOW); 
    coex->automaticCalibration();
    delay(1000);
    digitalWrite(led_signal, HIGH); 
}


//============
void test_2() {
    flicker_led(led_signal, 10, 200);
    digitalWrite(led_signal, LOW); 
    coex->automaticCalibration();
    // coex->calibration();
    digitalWrite(led_signal, HIGH);
    delay(15000);

    float target_speed = 7.5;
    coex->newLine(target_speed, true);
    float ret = 0;
    while (true) {
        ret = coex->followLine();
        if (ret == -2) {
            coex->stop();
            delay(1000);
        }
        if (ret == -1) {
            coex->stop();
            delay(1000);
        }
    }

//    float target_speed = 7.5;
//    coex->newLine(target_speed, true);
//    float ret = 0;
//    while (true) {
//        ret = coex->followLine();
//        if (ret == -2) {
//            coex->stop();
//            delay(15000);
//            coex->newLine(target_speed, true);
//        }
//        if (ret == -1) {
//            coex->stop();
//            delay(15000);
//            coex->newLine(target_speed, true);
//        }
//    }
}


//============
void test_3() {
    flicker_led(led_signal, 10, 200);
    digitalWrite(led_signal, LOW); 
    coex->calibration();
    digitalWrite(led_signal, HIGH);
    delay(15000);
    
    float target_speed = 7;
    coex->turnAlign(target_speed, 0, 0);
    delay(3000);
    coex->turnAlign(target_speed, 0, 0);
    delay(3000);
//    coex->turnAlign(target_speed, 2, 0);
//    delay(10000);
    
    coex->turnAlign(target_speed, 1, 0);
    delay(3000);
    coex->turnAlign(target_speed, 1, 0);
//    delay(3000);
//    coex->turnAlign(target_speed, 2, 0);
}
