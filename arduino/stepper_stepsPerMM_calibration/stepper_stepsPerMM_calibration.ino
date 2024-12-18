//stepper
#define stepPin 2
#define dirPin 4

#define limitPin 3

//lengte touw = 344cm
//stappen = 14342
int calibrate(){
  int counter = 0;
  digitalWrite(dirPin, HIGH);
  while(!digitalRead(limitPin)){
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
    counter++;
  }
  return counter;
}

void setup() {
  Serial.begin(9600);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(limitPin, INPUT);

  while(!digitalRead(limitPin)){
    Serial.println("Press limit switch to initiate...");
  }
  while(digitalRead(limitPin)){
    Serial.println("Release limit switch to start...");
  }
  int steps = calibrate();
  Serial.print("Amount of steps: ");
  Serial.println(steps);
}

void loop() {
}
