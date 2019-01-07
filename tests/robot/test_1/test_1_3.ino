void test_1_3() {
    // Turn four times to do full turn - adjust delay if needed
    for (int i = 0; i < 4; i++) {
        turn_right();
        delay(620);
    }

    // Turn four times to do full turn - adjust delay if needed
    for (int i = 0; i < 4; i++) {
        turn_left();
        delay(620);
    }
}
