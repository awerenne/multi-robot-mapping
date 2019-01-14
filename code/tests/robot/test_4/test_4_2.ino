/*
    Test sending of messages to master.
*/


//============
void test_4_2() {
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


//============
void send_msg(String msg) {
    String msg_with_header = "<" + String(id_slave) + "," + String(seq_number_slave) + "," + msg + ">\n";
    Bluetooth.println(msg_with_header);
    seq_number_slave += 1;
}








