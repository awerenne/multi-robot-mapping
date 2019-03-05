
//============
void test(byte test_id) {
    switch (test_id) {
        case 1: test_1(); break;
        case 2: test_2(); break;
        case 3: test_3(); break;
        case 4: test_4(); break;
    }
}



//============
void test_1() {
    /* 
        Check if bleutooth module is working
    */

    Serial.println("Start test");
    while (true) {
        delay(500);
        if (Bluetooth.available()) Serial.println("Message");
        else Serial.println("No message");
    }
}


//============
void test_2() {
    /*
        Simple reception test
    */

    Serial.println("Send data in this format <ID_SENDER, SEQ_NUMBER_SENDER, INSTRUCTION>");
    init_time = millis();
    while (millis() <= init_time + T) {
        recv_msg();  
        if (newData == true) {
            strcpy(tempChars, receivedChars);
            parse_instruction();
            show_msg_instruction();
            newData = false;
        }
    }
    Serial.println("End test"); 
    delay(5000); 
}



//============
void test_3() {
    /*
        Simple sending test
    */

    unsigned int f = 50;  
    int delay_ = 1000/f;  
    init_time = millis();
    while (init_time <= T + millis()) {
        send_msg(String(3.05));
        delay(delay_);
    }
    Serial.println("End test"); 
    delay(5000); 
}


//============
void test_4() {
    /*
        Sinus test
    */

    while (true) {
        recv_msg();  
        if (newData == true) {
            strcpy(tempChars, receivedChars);
            parse_instruction();
            send_msg(instruction);
            newData = false;
        }
    }
    delay(5000);  
}































