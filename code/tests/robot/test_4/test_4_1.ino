/*
    Test reception of messages from master, once received messages are
    parsed and displayed.
    
    Code heavily borrowed from 
        http://forum.arduino.cc/index.php?topic=396450 (example 5)
    Thanks!
*/


//============
void test_4_1() {
    Serial.println("Send data in this format <ID_SENDER, SEQ_NUMBER_SENDER, INSTRUCTION>  ");
    init_time = millis();
    while (millis() <= init_time + T) {
        recv_msg();  // Non-blocking reception
        if (newData == true) {
            // Temporary copy, because it is necessary to protect the original data
            // since strtok() used in parseData() replaces the commas with \0
            strcpy(tempChars, receivedChars);
            parse_instruction();
            show_msg_instruction();
            newData = false;
        }
    }
    Serial.println("End test"); 
    delay(5000); 
}


//============
void recv_msg() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Bluetooth.available() > 0 && newData == false) {
        rc = Bluetooth.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}


//============
void parse_instruction() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    id_master = atoi(strtokIndx);     // convert this part to an integer
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    seq_number_master = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    instruction = atoi(strtokIndx);     // convert this part to an integer

}


//============
void show_msg_instruction() {
    Serial.print("ID sender: ");
    Serial.println(id_master);
    Serial.print("Sequence #: ");
    Serial.println(seq_number_master);
    Serial.print("Instruction: ");
    Serial.println(instruction);
}


//============
void parse_pid_tuning() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    mode = atoi(strtokIndx);     // convert this part to an integer (0 -> start/stop, 1 -> tune pid)
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    kp = atof(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    kd = atof(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    ki = atof(strtokIndx);     // convert this part to a float

}


//============
void show_msg_pid_tuning() {
    Serial.print("Mode: ");
    Serial.println(mode);
    Serial.print("Kp: ");
    Serial.println(kp);
    Serial.print("Kd: ");
    Serial.println(kd);
    Serial.print("Ki: ");
    Serial.println(ki);
}

























