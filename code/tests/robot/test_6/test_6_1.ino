/*
    Description.
*/


//============
void test_6_1() {
    while (true) {
        bool new_msg = messenger->ReceiveMessage();  
        if (new_msg == true) {
            messenger->ParseInstruction();  // received messages are in the following format: <id_sender, seq_number_sender, instruction>
            instruction = messenger->GetInstruction();
        }
    
        switch (instruction) {
            case 0:  // Stop
                actuators->Stop();
                digitalWrite(led_running, LOW);
                break; 
            case 1:  // Go forward
                actuators->UpdateSpeed(60, 60);
                digitalWrite(led_running, HIGH);
                break;
            case 2:  // Turn left
                actuators->Turn(false, false);
                digitalWrite(led_running, HIGH);
                instruction = 0;
                break;
            case 3:  // Turn right
                actuators->Turn(true, false);
                digitalWrite(led_running, HIGH);
                instruction = 0;
                break;
            case 4:  // Full turn
                actuators->Turn(false, true);
                digitalWrite(led_running, HIGH);
                instruction = 0;
                break;
        }
        
        delay(250);
    }
}








