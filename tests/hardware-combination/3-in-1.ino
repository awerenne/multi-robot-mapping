#include <Stepper.h>


const int pin_sensor_left = 2;
const int pin_sensor_middle = 3;
const int pin_sensor_right = 4;

const int pin_bleutooth_rx = 5;
const int pin_bleutooth_tx = 6;
const int pin_bleutooth_button = 7;
const int pin_bleutooth_led_rcv = 8;
const int pin_bleutooth_led_snd = 9;

int pin_motor_1 = 10;
int pin_motor_2 = 11;
int pin_motor_3 = 12;
int pin_motor_4 = 13; 

const int stepsPerRevolution = 64;  
Stepper myStepper(stepsPerRevolution, pin_motor_1, pin_motor_2, pin_motor_3, pin_motor_4);
int stepCount = 0;  // number of steps the motor has taken


void setup() {
  pinMode(pin_sensor_left, INPUT_PULLUP);
  pinMode(pin_sensor_middle, INPUT_PULLUP);
  pinMode(pin_sensor_right, INPUT_PULLUP);
  pinMode(pin_bleutooth_led_rcv, OUTPUT);

  Serial.begin(9600);
}


int sensors_vector[3];

void loop() {
  // Read sensors
  sensors_vector[0] = digitalRead(pin_sensor_left);
  sensors_vector[1] = digitalRead(pin_sensor_middle);
  sensors_vector[2] = digitalRead(pin_sensor_right);

  displaySensors();
  digitalWrite(pin_bleutooth_led_rcv, HIGH);

  // map it to a range from 0 to 100:
    //myStepper.setSpeed(50);
    // step 1/100 of a revolution:
    //myStepper.step(stepsPerRevolution / 100);
  delay(2000);
}


void displaySensors() {
  // Display sensors vector
  for (int i = 0; i < 3; i++) {
    Serial.print(sensors_vector[i]);
    Serial.print("   ");
  }
  Serial.print("\n");
}
