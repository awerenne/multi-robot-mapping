void test_8_1() {
    // Test where the goal is to follow a straight line until intersection is detected, determine which intersection and turn to desired segment

    int iteration = 0;
    while (true) {
        // Wait for instruction to start
        while (instruction != 1) {
            bool new_msg = messenger->ReceiveMessage();  
            if (new_msg == true) {
                messenger->ParseInstruction();
                instruction = messenger->GetInstruction();
            }
        }
    
        // Follow line until intersection
        while (instruction == 1) {
            sensors->QTRARead();
            int error = sensors->GetError();
            actuators->FollowLine(error);
            
            if ((iteration%2) == 0 && is_intersection())
                break;
    
            if ((iteration%5) == 0 && sensors->IsObstacle()) {
                actuators->Stop();
                instruction = 0;
            }
    
            delay(10);
        }
    
        // Determine type of intersection
        byte type_ = type_intersection();
        messenger->SendMessage(String(type_));
        left_hand_rule(type_intersection);
        instruction = 0;
        iteration++;
    }
}
