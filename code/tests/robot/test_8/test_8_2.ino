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


//============
bool is_intersection() {
    sensors->QTRARead();
    unsigned int* temp = sensors->GetQTRValues();
    bool is_road_left = (temp[5] > 800);
    bool is_road_here = (temp[2] > 800 || temp[3] > 800);
    bool is_road_right = (temp[0] > 800);
    if (is_road_left || is_road_right || !is_road_here) {
        actuators->Stop();
        flicker_led(led_signal, 20, 50);
        digitalWrite(led_signal, HIGH);
        return true;
    } 
    return false;
}


//============
byte type_intersection() {
    unsigned int* temp = sensors->GetQTRValues();
    bool is_road_left = (temp[5] > 800);
    bool is_road_here = (temp[2] > 800 || temp[3] > 800);
    bool is_road_right = (temp[0] > 800);

    one_inch();
    sensors->QTRARead();

    temp = sensors->GetQTRValues();
    is_road_front = (temp[2] > 800 || temp[3] > 800);

    if (is_road_left && is_road_front && is_road_right)
        return 0;
    else if (is_road_left && is_road_front && !is_road_right)
        return 1;
    else if (!is_road_left && is_road_front && is_road_right)
        return 2;
    else if (is_road_left && !is_road_front && is_road_right)
        return 3;
    else if (is_road_left && !is_road_front && !is_road_right)
        return 4;
    else if (!is_road_left && !is_road_front && is_road_right)
        return 5;
    return 6;
}


//============
void one_inch() {
    actuators->UpdateSpeed(60,60);
    delay(850);
    actuators->Stop();
}


//============
void left_hand_rule(byte type_intersection) {
    switch (type_intersection) {
        case 0:
            actuators->Turn(false, false);
            align(false);
            break;
        case 1:
            actuators->Turn(false, false);
            align(false);
            break;
        case 3: 
            actuators->Turn(false, false);
            align(false);
            break;
        case 4:
            actuators->Turn(false, false);
            align(false);
            break;
        case 5:
            actuators->Turn(true, false);
            align(true);
            break;
        case 6:
            actuators->Turn(true, true);
            align(true);
            break;
    }
    delay(200);
}


//============
void align(bool clockwise) {
    if (clockwise)
        actuators->UpdateSpeed(60, -60);
    else
        actuators->UpdateSpeed(-60, 60);
    int error;
    do {
        delay(5);
        sensors->QTRARead();
    } while (!sensors->IsAligned(clockwise)); //  ALIGN
    actuators->Stop();
    delay(200);
}
