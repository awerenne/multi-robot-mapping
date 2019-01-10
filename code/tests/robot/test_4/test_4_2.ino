void test_4_2() {
    // Test sending of messages to master
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
