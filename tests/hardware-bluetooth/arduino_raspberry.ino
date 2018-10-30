//#include <SoftwareSerial.h>
//
//SoftwareSerial btSerial(10, 11); // RX, TX
//
////Tells us we are sending over Serial
//const int senderPin = 7;
//
////Tells us we are receiving confirmation over Serial
//const int receiverPin = 8;
//unsigned long timer = 0;
//int sendStatus = 1;
//
//void setup() {
//  
//  pinMode(senderPin, OUTPUT);
//  pinMode(receiverPin, OUTPUT);
//  
//  //Setup and flush the serials to begin
//  btSerial.begin(9600);
//  Serial.begin(9600);
//  btSerial.flush();
//  Serial.flush();
//}
//
//void loop() {
//    
//  //I want to send the temperature, I want the Raspberry PI to grab it, process it
//  //and send back a message. I don't want to continue spamming the raspberry pi, so
//  //I will only send a signal every 10 seconds.
//  
//  //Non-blocking every 10 seconds.
//  if ((timer == 0 || millis() >= timer) && sendStatus == 1){
//    
//    //Send the current temperature. Didn't use readline to avoid blocking problem
//    String sendDegrees = "<yo>";
//    
//    //Convert to byte array
//    char charArray[sendDegrees.length() + 1];
//    sendDegrees.toCharArray(charArray, sendDegrees.length()+1);
//    
//    btSerial.write(charArray);
//    blinkTransmit(5, senderPin);
//    
//    //Reset the timer for another 10 seconds.
//    timer = millis() + 10000;
//      
//  }
//
//  //Received message back from Raspberry.
//  //RPI sends back messages in the form of <5>.
//  //We only check for the numerical.
//  while (btSerial.available()) {
//    //Numbers are between 48 and 57
//    char rpiMessage = btSerial.read();
//    //Serial.println(rpiMessage); //Shows message from RPi.
//    
//    if (rpiMessage >= 48 && rpiMessage <= 57) {
//      blinkTransmit(rpiMessage - '0', receiverPin); //convert to int before sending
//    }
//    
//  }
//    
//}
//
////Function used for blinking the sending and receiving pins.
////Just pass the appropriate pins as arguments.
//void blinkTransmit(int numberOfTimes, int senderReceiver) {
//  for (int i = 0; i < numberOfTimes; i++) {
//    digitalWrite(senderReceiver, HIGH);
//    delay(50);
//    digitalWrite(senderReceiver, LOW);
//    delay(50);
//  }
//}
