void test_4_1() {
    // Test reception of messages from master
    // Once recepted, messages are parsed and displayed
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
