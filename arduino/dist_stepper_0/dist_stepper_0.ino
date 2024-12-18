//stepper
const uint8_t stepPin = 2; //blauw draadje
const uint8_t dirPin = 4;
const uint8_t STEPS_PER_REV = 200;
const float SPOOL_RADIUS = 7.5;

//const double STEPS_PER_MM = STEPS_PER_REV / (6.28318530717959 * SPOOL_RADIUS);
const double STEPS_PER_MM = 4.169186;

//dist sensor
const uint8_t trigPin = 5;
const uint8_t echoPin = 6;
const uint8_t treshold = 0; //----------------- nog in te stellen (in mm)-----------------

//buttons
const uint8_t limitPin = 3;
const uint8_t pressurePin = 7;

//rgb
const uint8_t redPin = 11;
const uint8_t greenPin = 10;
const uint8_t bluePin = 9;

int dist = 0;
bool gameStarted = LOW;

//stepping function
void step(int steps, int speed, bool locking) { //speed rond 800 zetten
  digitalWrite(dirPin, (steps > 0));
  steps = abs(steps);
  for(int i = 0; i < steps; i++){
    digitalWrite(stepPin, !locking);
    delayMicroseconds(speed);
    digitalWrite(stepPin, locking);
    delayMicroseconds(speed);
  }
}

void setColor(byte red, byte green, byte blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void calibrate(){
  Serial.println("calibrating...");
  digitalWrite(dirPin, HIGH);
  while(digitalRead(limitPin)){
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
  }
  Serial.println("done!");
}

int distance(){ //takes about 200µs
  int distance = 0;
  int duration = 0;
  for(int i = 0; i < 5; i++){
    digitalWrite(trigPin, LOW);
    delay(2);
    
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    
    float timing = pulseIn(echoPin, HIGH);
    distance = (timing * 0.034) / 2;
  }
  return distance*2;
}

void setup() {
  Serial.begin(9600);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(limitPin, INPUT);
  pinMode(pressurePin, INPUT);

  calibrate();
}

void loop() {
  if(!pressurePin){
    setColor(0, 255, 0);
    delay(20);
  }

  if (digitalRead(pressurePin) && !gameStarted){
  setColor(255, 0, 0);

    while(dist > treshold){
      dist = distance();
      Serial.print("distance: ");
      Serial.println(dist);

      dist = min(10, dist);
      int steps = (int) - (dist * STEPS_PER_MM);
      Serial.println(steps);
      
      step(steps, 800, HIGH);
    }

    gameStarted = HIGH;

    //keypress c ofz om de game te starten
  }
  if(gameStarted){
    Serial.println("start");
    //wacht tot game gedaan is, msg in serial reader mss? (emuleer met knop)
  }
}

/* led kleurtjes
wachten op een user: groen
user op sensor: rood
game start: fade in fade out blauw
*/

