//stepper
const uint8_t stepPin = 2;  //blauw draadje
const uint8_t dirPin = 4;
const uint8_t STEPS_PER_REV = 200;
const float SPOOL_RADIUS = 7.5;

//const double STEPS_PER_MM = STEPS_PER_REV / (6.28318530717959 * SPOOL_RADIUS);
const double STEPS_PER_MM = 4.169186;

//dist sensor
const uint8_t trigPin = 5;
const uint8_t echoPin = 6;
const uint8_t treshold = 100;  //----------------- nog in te stellen (in mm)------------------------------

//buttons
const uint8_t limitPin = 3;
#define pressurePin A1

//rgb
const uint8_t redPin = 11;
const uint8_t greenPin = 10;
const uint8_t bluePin = 9;

int dist = 0;
int state = 0;

byte fadeVal = 0;
bool fadeDir = HIGH;

//timing
uint32_t t0 = 0;
uint32_t t1 = 0;

//stepping function
bool step(int steps, int speed, bool locking) {  //speed rond 800 zetten
  digitalWrite(dirPin, (steps > 0));
  steps = abs(steps);
  for (int i = 0; i < steps; i++) {
    if (!digitalRead(limitPin)) {
      digitalWrite(stepPin, LOW);
      delay(100);  // touw laten zakken door zwaartekracht.
      digitalWrite(stepPin, HIGH);
      calibrate();
      return HIGH;
    }
    digitalWrite(stepPin, !locking);
    delayMicroseconds(speed);
    digitalWrite(stepPin, locking);
    delayMicroseconds(speed);
  }
  return LOW;
}

void setColor(byte red, byte green, byte blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void calibrate() {
  Serial.println("calibrating...");
  digitalWrite(dirPin, HIGH);
  while (digitalRead(limitPin)) {
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
  }
  Serial.println("done!");
}

int distance() {  //takes about 200µs
  int distance = 0;
  int duration = 0;
  for (int i = 0; i < 5; i++) {
    digitalWrite(trigPin, LOW);
    delay(2);

    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);

    float timing = pulseIn(echoPin, HIGH);
    distance = (timing * 0.034) / 2;
  }
  return distance * 2;
}

void stapAf() {
  t0 = millis();
  bool flashState = LOW;
  while (digitalRead(pressurePin)) {
    t1 = millis();
    if (t0 - t1 >= 200) {
      t0 = millis();
      flashState = !flashState;
    }

    if (flashState) {
      setColor(255, 0, 0);
    } else {
      setColor(0, 0, 0);
    }
  }
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
  t1 = millis();

  switch (state) {
    case 0:  // wachten tot user 2s stilstaat
      setColor(0, 255, 0);
      if (digitalRead(pressurePin)) {
        if (t1 - t0 >= 2000) {
          state = 1;
        }
      } else {
        t0 = millis();
      }
      break;

    case 1:  // headset komt op de juiste hoogte
      setColor(255, 0, 0);
      while (dist > treshold) {
        dist = distance();
        Serial.print("distance: ");
        Serial.println(dist);

        dist = min(10, dist);  // arbitrair 10 gekozen
        int steps = (int)(-dist * STEPS_PER_MM);
        Serial.println(steps);

        if (step(steps, 800, HIGH)) {
          state = 6;  // error handling
          break;
        }
      }

      if (step(200, 800, HIGH)) {  // nog een paar cm zakken (JUISTE STEPS NOG IN TE VULLEN) ------------------------------------------------------------------------------
        state = 6;  // error handling
        break;
      }

      Serial.println("s");  // game start commando (iets van safety toevoegen voor 100% zekere overdracht?)
      state = 2;
      fadeVal = 0;
      fadeDir = HIGH;
      t0 = millis();
      break;

    case 2:  //game is bezig, wacht op serial bus commando
      if (t1 - t0 >= 20) {
        t0 = millis();
        if (fadeDir) {
          fadeVal++;
        } else {
          fadeVal--;
        }
        if (fadeVal == 255) {
          fadeDir = LOW;
        } else if (fadeVal == 0) {
          fadeDir = HIGH;
        }
      }
      setColor(0, 0, fadeVal);

      if (Serial.available() > 0) {
        int input = (char)Serial.read();  // wachten op "e" in de Serial bus (stop signal)
        if (input == 'e') {
          state = 3;
        }
      }
      break;

    case 3:  //game is over, wachten tot user is afgestapt, dan bril optillen.
      stapAf();
      // meten ofdat bril is afgezet met distance sensor!!!
      if (step(100, 800, HIGH)) {
        state = 6;  // error handling
        break;
      }
      delay(1000);
      calibrate();
      state = 0;
      break;

    case 6:  //bril tegen endstop gebotst, user van platform laten stappen, kleur flikkerend rood totdat user van het platform stapt. (index kan nog veranderen)
      stapAf();
      state = 0;
      break;
  }
}

/* led kleurtjes
wachten op een user: groen / hue shift (rave mode)
user op sensor: rood
game start: fade in fade out blauw
leave platform: flikkerend rood
*/
