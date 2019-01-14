/*
    Test distance ground truth versus estimated
    compare several algorithms (naive, ...)
*/


//============
void test_8_3() {
    sensors->AutomaticCalibrate();

    float ground_truth = 0;
    for (int ground_truth = 5; ground_truth < 30; ground_truth+=5) {
        run(ground_truth);
        flicker_led(led_running, 15, 800);
        digitalWrite(led_running, LOW); 
    }
}


//============
void run(float ground_truth) {
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
            
            digitalWrite(led_running, HIGH);
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

            // Check if intersection
            if (is_intersection()) {
                String msg = String(ground_truth) + ";" + String(speed) + ";" + String(diff_time);
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
