//stepper
#define stepPin 2
#define dirPin 4
#define STEPS_PER_REV 200
#define SPOOL_RADIUS 7.5
const int offset = 300;

//const float STEPS_PER_MM = STEPS_PER_REV / (6.28318530717959 * SPOOL_RADIUS);
const float STEPS_PER_MM = 2.9184302;

//dist sensor
#define trigPin 5
#define echoPin 6

#define limitPin 3

float height;
float dist = 0;

//stepping function
bool step(int steps, int speed, bool locking) { //speed rond 800 zetten
  digitalWrite(dirPin, (steps > 0));
  steps = abs(steps);
  for(int i = 0; i < steps; i++){
    if(!digitalRead(limitPin)){
      height = offset;
      return LOW;
    }
    digitalWrite(stepPin, !locking);
    delayMicroseconds(speed);
    digitalWrite(stepPin, locking);
    delayMicroseconds(speed);
  }
  return HIGH;
}

void calibrate(){
  digitalWrite(dirPin, HIGH);
  Serial.println("calibrating...");
  while(digitalRead(limitPin)){
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
  }
  Serial.println("done calibrating!");
  height = offset;
}

float distance(){ //takes about 200µs
  float distance = 0;
  float duration = 0;
  for(int i = 0; i < 5; i++){

    digitalWrite(trigPin, LOW);
    delay(2);
    
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distance += (duration * 0.034) / 2;
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

  calibrate();
}

void loop() {
  //include PID?
  //speed = map(abs(dist_0 - dist1);
  dist = distance();

  Serial.print("height: ");
  Serial.println(height);
  Serial.print("distance: ");
  Serial.println(dist);

  int steps = (int) (height - dist) * STEPS_PER_MM;
  Serial.println(steps);
  /*
  PID idee => werken per 10 stappen.
  steps = min(steps, 10);
  height += steps*STEPS_PER_MM;
  int speed = map(abs(height - dist), 0, 5000, 2500, 800);
  */
  if (step(steps, 800, HIGH)){
    height = dist;
  }
}
