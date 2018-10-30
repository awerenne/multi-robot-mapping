#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
}

void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}




//
//
//// This program shown how to control arduino from PC Via Bluetooth
//// Connect ...
//// arduino>>bluetooth
//// D11   >>>  Rx
//// D10   >>>  Tx
////Written By Mohannad Rawashdeh
////for http://www.genotronex.com/
//
//// you will need arduino 1.0.1 or higher to run this sketch
//
//#include <SoftwareSerial.h>// import the serial library
//
//SoftwareSerial Genotronex(10, 11); // RX, TX
//int ledpin=8; // led on D13 will show blink on / off
//int BluetoothData; // the data given from Computer
//
//void setup() {
//  // put your setup code here, to run once:
//  Genotronex.begin(9600);
//  Genotronex.println("Bluetooth On please press 1 or 0 blink LED ..");
//  pinMode(ledpin,OUTPUT);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//   if (Genotronex.available()){
//BluetoothData=Genotronex.read();
//   Genotronex.println("yo"); 
//   if(BluetoothData=='1'){   // if number 1 pressed ....
//   digitalWrite(ledpin,HIGH);
//   Genotronex.println("LED  On D13 ON ! ");
//   }
//  if (BluetoothData=='0'){// if number 0 pressed ....
//  digitalWrite(ledpin,LOW);
//   Genotronex.println("LED  On D13 Off ! ");
//  }
//}
//delay(100);// prepare for next data ...
//}
