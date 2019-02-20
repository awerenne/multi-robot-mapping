/*
    Simple motor manipulation.
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
        case 3:
            test_3();
            break;
    }
}



/*
    Test where the goal is to follow a straight line until intersection is detected, determine which intersection and turn to desired segment.
    Problem: Works at first intersection but when restart never sees second intersection!!!
*/


//============
void test_8_2() {

    sensors->AutomaticCalibrate();

    int iteration = 0;
    while (true) {
        bool new_msg = messenger->ReceiveMessage();  
        if (new_msg == true) {
            messenger->ParseInstruction();
            instruction = messenger->GetInstruction();
            iteration = 0;
        }
        
        if (instruction == 0)
            actuators->Stop();

        // Follow line until intersection
        if (instruction == 1) {
            sensors->QTRARead();
            int error = sensors->GetError();
            actuators->FollowLine(error);
            
             if ((iteration%2) == 0 && is_intersection())
                 break;
    
            delay(10);
            iteration++;
        }
    }

    byte type_ = type_intersection();
    messenger->SendMessage(String(type_));
    left_hand_rule(type_);
    messenger->SendMessage("aligned");
}


/*
    Test distance ground truth versus estimated
    compare several algorithms (naive, ...)
*/


//============
void test_8_3() {
    sensors->AutomaticCalibrate();

    float distance = 0;
    while (true) {
        bool new_msg = messenger->ReceiveMessage();  
        if (new_msg == true) {
            messenger->ParseInstruction();
            instruction = messenger->GetInstruction();
        }
    
        // Follow line
        if (instruction != 0) {  
            float start_time = millis();
            sensors->InitEncoders();
            
            digitalWrite(led_signal, HIGH);
            sensors->QTRARead();
            int error = sensors->GetError();
            actuators->FollowLine(error);
            delay(10);

            // Compute distance
            float speed_left = sensors->GetSpeedLeft();
            float speed_right = sensors->GetSpeedRight();
            float diff_time = (float) (millis() - start_time);
            float speed = (speed_left + speed_right)/2;
            distance += compute_distance_naive(speed, diff_time); 
            sensors->StopEncoders();

            // Check if intersection
            if (is_intersection()) {
                String msg = String(distance);
                messenger->SendMessage(msg);
                return;
            }
        }
    }
}



// Determine trajectory with error and distance computed with initial pertubation


























