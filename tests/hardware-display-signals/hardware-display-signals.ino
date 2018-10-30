const int di_leds[2] = {8,9};
const byte sequence[8] = {0,1,0,1,0,1,1,0};
const int duration = 400;

void setup() {
  pinMode (di_leds[0], OUTPUT);
  pinMode (di_leds[1], OUTPUT);
}

void loop() {
  
  for (int i = 0; i < 8; i++) {
    int j = sequence[i];
    delay (duration);
    digitalWrite(di_leds[j], HIGH);   
    delay(duration);               
    digitalWrite(di_leds[j], LOW);    
    delay(duration); 
  }
}
