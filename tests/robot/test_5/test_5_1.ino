void test_5_1() {
    // Test to check if encoder counts 12 per revolution (for each wheel separate and then together)
    // Observations seems to corresponds to expected theoretic results (6 * 150.58 = 903.48 counts per revolution) -- if we counts both edges on one channel
    
    int running_time = 800;
    Serial.println(String(counter_left) + ", " + String(counter_right)); 
    
    actuators->UpdatePWM(70, 0);
    delay(running_time);  
    actuators->Stop();
    delay(1000);  // Wait to be completely stopped

    Serial.println(String(counter_left) + ", " + String(counter_right));

    actuators->UpdatePWM(0, 80);
    delay(running_time);
    actuators->Stop();
    delay(1000);  // Wait to be completely stopped

    Serial.println(String(counter_left) + ", " + String(counter_right));

    actuators->UpdatePWM(70, 80);
    delay(running_time);
    actuators->Stop();
    delay(1000);  // Wait to be completely stopped

    Serial.println(String(counter_left) + ", " + String(counter_right));
    delay(2000);  // Wait to be sure results are printed before quitting 
}
