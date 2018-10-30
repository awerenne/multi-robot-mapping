#include <Servo.h>

Servo servo_left;
Servo servo_right;  

int pos = 0;    // variable to store the servo position

void setup() {
  servo_left.attach(12); 
  servo_right.attach(13);  
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo_left.write(pos);              // tell servo to go to position in variable 'pos'
    servo_right.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo_left.write(pos);              // tell servo to go to position in variable 'pos'
    servo_right.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
