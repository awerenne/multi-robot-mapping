/*
    Test reception/sending of messages from/to master.
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
    }
}



//============
void test_1() {
    Serial.println("Send data in this format <ID_SENDER, SEQ_NUMBER_SENDER, INSTRUCTION>  ");
    init_time = millis();
    while (millis() <= init_time + T) {
        recv_msg();  // Non-blocking reception
        if (newData == true) {
            // Temporary copy, because it is necessary to protect the original data
            // since strtok() used in parseData() replaces the commas with \0
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
void test_2() {
    unsigned int f = 500;  // frequency
    int delay_ = 1000/f;  
    init_time = millis();
    while (init_time<= T + millis()) {
        send_msg(String(3.05));
        delay(delay_);
    }
    Serial.println("End test"); 
    delay(5000); 
}


// Test frequency limit (higher than that receiving frequency data will be lost
// due to the fact it can not process fast enough)








