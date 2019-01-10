void test_8_2() {
    // Test distance ground truth versus estimated
    // compare several algorithms (naive, ...)

    float distance = 0;
    float ground_truth = 0;
    unsigned int delay_ = 10;
    float speed_left = 0, speed_right = 0;
    unsigned long previous_time_left = millis();
    unsigned long previous_time_right = millis();
    unsigned long diff_time = millis() - previous_time_left;
    
    for (int ground_truth = 5; ground_truth < 30; ground_truth++)
        run(ground_truth);
}


//============

void run(float ground_truth) {
    while (true) {
        bool new_msg = messenger->ReceiveMessage();  
        if (new_msg == true) {
            messenger->ParseInstruction();
            instruction = messenger->GetInstruction();
        }
    
        // Follow line
        if (instruction != 0) {  
            counter_left = 0;
            previous_time_left = millis();
            counter_right = 0;
            previous_time_right = millis();

            digitalWrite(led_running, HIGH);
            sensors->QTRARead();
            int error = sensors->GetError();
            actuators->FollowLine(error);
            delay(delay_);

            // Compute speed
            diff_time = (float) (millis() - previous_time_left)
            speed_left = alpha * counter_left / diff_time;
            speed_right =  alpha * counter_right / diff_time;

            // Compute distance
            float speed = 0.5 * (speed_left + speed_right);
            distance += compute_distance(speed, diff_time); 

            // Check if intersection
            if (is_intersection()) {
                String msg = String(ground_truth) + ";" + String(distance);
                messenger->SendMessage(msg);
                return;
            }
        }
    }
}


//============

float compute_distance_naive(float speed, unsigned long diff_time) {
    return (speed * ((float) diff_time));
}










