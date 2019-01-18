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


//============
float compute_distance_naive(float speed, unsigned long diff_time) {
    return (speed * ((float) diff_time))/7474.322;  // This factor was not computed but simply by matching to results
    // TODO: understand 
}
