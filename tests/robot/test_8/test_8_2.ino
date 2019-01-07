void test_8_2() {
    // Test autonomous exploration of maze by robot (Algorithm: left-hand rule)

    unsigned int iteration = 0;
    while (true) {
        bool new_msg = messenger->ReceiveMessage();  
        if (new_msg == true) {
            messenger->ParseInstruction();
            instruction = messenger->GetInstruction();
        }
    
        if (instruction == 0) {  // Stop 
            actuators->Stop();
            digitalWrite(led_running, LOW);
        }
        else {  // Maze exploration
            digitalWrite(led_running, HIGH);
            sensors->QTRARead();
            int error = sensors->GetError();
            actuators->FollowLine(error);

            // Check if intersection
            if ((iteration%2) == 0 && is_intersection()) {
                byte type_ = type_intersection();
                messenger->SendMessage(String(type_));
                left_hand_rule(type_);
            }

            // Check if obstacle
            if ((iteration%5) == 0 && sensors->IsObstacle()) {
                actuators->Stop();
                instruction = 0;
            }
            
            delay(10);
        }
        iteration++;
    }
}
