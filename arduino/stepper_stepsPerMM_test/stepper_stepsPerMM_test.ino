const int stepPin = 3; 
const int dirPin = 4; 

int counter = 0;

void setup() {
  // Sets the two pins as Outputs
  Serial.begin(9600);
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(AO, INPUT);

}
void loop() {
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 1000; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(200); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(200); 
  }
}
