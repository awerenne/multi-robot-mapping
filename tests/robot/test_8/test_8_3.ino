void test_8_3() {
    // Test autonomous exploration of maze by robot (Algorithm: left-hand rule)

    unsigned int iteration = 0;
    float distance = 0;
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
            counter_left = 0;
            previous_time_left = millis();
            counter_right = 0;
            previous_time_right = millis();

            digitalWrite(led_running, HIGH);
            sensors->QTRARead();
            int error = sensors->GetError();
            actuators->FollowLine(error);
            
            // Compute speed
            diff_time = (float) (millis() - previous_time_left)
            speed_left = alpha * counter_left / diff_time;
            speed_right =  alpha * counter_right / diff_time;

            // Compute distance
            float speed = 0.5 * (speed_left + speed_right);
            distance += compute_distance_naive(speed, diff_time); 

            // Check if intersection
            if ((iteration%2) == 0 && is_intersection()) {
                byte type_ = type_intersection();
                String msg = String(type_) + ";" + String(distance);
                messenger->SendMessage(msg);
                left_hand_rule(type_);
            }

            // Check if obstacle
            else if ((iteration%5) == 0 && sensors->IsObstacle()) {
                actuators->Stop();
                String msg = "Y" + ";" + String(distance);
                messenger->SendMessage(msg);
                instruction = 0;
            }

            // Every now and then send update of travelled distance
            else if ((iteration%100) == 0) {
                String msg = "Z" + ";" + String(distance);
                messenger->SendMessage(msg);
                distance = 0;
            }
            
            delay(10);
        }
        distance = 0;
        iteration++;
    }
}