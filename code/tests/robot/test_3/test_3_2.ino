void test_3_2() {
    String measure;
    while (true) {
        QTRARead();
        measure = String(isRoadLeft()) + " ";
        measure += String(isRoadMiddle()) + " ";
        measure += String(isRoadRight());
        Serial.println(measure);
        delay(1000);
    }
}
