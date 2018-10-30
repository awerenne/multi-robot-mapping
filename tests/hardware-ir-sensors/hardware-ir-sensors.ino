
const int do_sensors[3] = {2,3,4};

void setup() {
  pinMode(do_sensors[0], INPUT_PULLUP);
  pinMode(do_sensors[1], INPUT_PULLUP);
  pinMode(do_sensors[2], INPUT_PULLUP);
  Serial.begin(9600);
}

int sensors_vector[7];

void loop() {
  
  // Read sensors
  sensors_vector[2] = digitalRead(do_sensors[0]);
  sensors_vector[3] = digitalRead(do_sensors[1]);
  sensors_vector[4] = digitalRead(do_sensors[2]);

  // Display sensors vector
  for (int i = 2; i < 5; i++) {
    Serial.print(sensors_vector[i]);
    Serial.print("   ");
  }
  Serial.print("\n");

  // Wait 
  delay(2000);
}
