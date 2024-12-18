#include <Servo.h>

#define MAX 180
#define MIN 0

int val; // for incoming serial data

Servo myservo;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  myservo.attach(9);
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:

    val = Serial.readString().toInt();
    val = max(min(val, MAX), MIN); // send outer limits on [MIN; MAX]
    
    // say what you got:
    Serial.print("I received: ");
    Serial.println(val);
    myservo.write(val);
  }
}