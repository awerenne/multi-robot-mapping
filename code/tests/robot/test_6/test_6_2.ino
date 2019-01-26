/*
    Description.
*/


//============
void test_6_2() {
    while (true) {
        bool new_msg = messenger->ReceiveMessage();  
        if (new_msg == true) {
            messenger->ParseInstruction();  // received messages are in the following format: <id_sender, seq_number_sender, instruction>
            instruction = messenger->GetInstruction();
        }
    
        switch (instruction) {
            case 0:  // Stop
                actuators->Stop();
                digitalWrite(led_signal, LOW);
                break; 
            case 1:  // Go forward
                actuators->UpdateSpeed(60, 60);
                digitalWrite(led_signal, HIGH);
                break;
        }
    
        if (sensors->IsObstacle()) {
            actuators->Stop();
            instruction = 0;
        }
        
        delay(250);
    }
}
