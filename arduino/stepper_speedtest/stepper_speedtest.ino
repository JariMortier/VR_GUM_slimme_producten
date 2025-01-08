const int stepPin = 2; 
const int dirPin = 4; 

int speed = 800;

void setup() {
  // Sets the two pins as Outputs
  Serial.begin(9600);
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(A0, INPUT);
}
void loop() {
  digitalWrite(dirPin, HIGH);
  for(int i = 0; i < 1000; i++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
  digitalWrite(stepPin, HIGH);
  digitalWrite(dirPin, LOW);
  for(int i = 0; i < 1000; i++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
  digitalWrite(stepPin, HIGH);
}
