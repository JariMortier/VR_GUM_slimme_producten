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
const int treshold = 500;
const int pressureTreshold = 350;

//buttons
const uint8_t limitPin = 3;
#define pressurePin A0

//rgb
const uint8_t redPin = 11;
const uint8_t greenPin = 10;
const uint8_t bluePin = 9;

double dist = 0.0;
double ceilHeight = 0.0;
int state = 0;

byte fadeVal = 0;
bool fadeDir = HIGH;
double hue = 0.0;

double sinVal = 0.0;

//timing
uint32_t t0 = 0;
uint32_t t1 = 0;
uint32_t t_reset = 0;
const int resetTime = 10000;

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

void setColorRGB(byte red, byte green, byte blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void setColorHSL(double h, double s, double l) {
  double r, g, b, p, q;

  if (s == 0) {
    r = g = b = l; // achromatic
  } else {
    if(l < 0.5){
      q = l + s * l;
    } else {
      q = l + s - l * s;
    }

    p = 2 * l - q;

    r = hueToRgb(p, q, h + 1.0/3);
    g = hueToRgb(p, q, h);
    b = hueToRgb(p, q, h - 1.0/3);
  }

  setColorRGB(round(r * 255), round(g * 255), round(b * 255));
}

double hueToRgb(double p, double q, double t) {
  if (t < 0){
    t += 1;
  }
  if (t > 1){
    t -= 1;
  }
  if (t < 1.0/6){
    return p + (q - p) * 6 * t;
  }
  if (t < 1.0/2){
    return q;
  }
  if (t < 2.0/3){
    return p + (q - p) * (2.0/3 - t) * 6;
  }
  return p;
}

void calibrate() {
  digitalWrite(dirPin, HIGH);
  while (digitalRead(limitPin)) {
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
  }
  digitalWrite(dirPin, LOW);
  while (!digitalRead(limitPin)) {
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
  }
}

double distance() {  //takes about 200µs
  double distance = 0;
  for (int i = 0; i < 5; i++) {
    digitalWrite(trigPin, LOW);
    delay(2);

    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);

    double timing = pulseIn(echoPin, HIGH);
    distance += (timing * 0.034) / 2;
  }
  return distance * 2;
}

void stapAf() {
  t0 = millis();
  bool flashState = LOW;
  while (analogRead(pressurePin) > pressureTreshold) {
    t1 = millis();
    if (t1 - t0 >= 300) {
      t0 = millis();
      flashState = !flashState;
    }

    if (flashState) {
      setColorRGB(255, 0, 0);
    } else {
      setColorRGB(0, 0, 0);
    }
  }
}

void zetBrilAf() {
  t0 = millis();
  bool flashState = LOW;
  bool check = HIGH;
  int counter = 0;
  while (check) {
    t1 = millis();
    if (t1 - t0 >= 200) {
      t0 = millis();
      flashState = !flashState;
    }
    if (flashState) {
      setColorRGB(255, 0, 0);
    } else {
      setColorRGB(0, 0, 0);
    }

    double dist = distance();
    if (dist >= treshold && dist < ceilHeight){ // 5 consecutive height checks
      counter++;
    } else {
      counter = 0;
    }

    if(counter >= 5){
      check = LOW;
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

  // meet plafond hoogte
  ceilHeight = 0.0;
  for(int i = 0; i < 10; i++){
    ceilHeight += distance();
  }
  ceilHeight /= 10;
  t_reset = millis();
  state = 0;
}

void loop() {
  t1 = millis();

  switch (state) {
    case 0:  // wachten tot user 2s stilstaat
      setColorHSL(hue, 1, 0.5);
      hue += 0.00002;
      if(hue > 1){
        hue -= 1;
      }

      if (analogRead(pressurePin) > pressureTreshold) {
        if (t1 - t0 >= 2000) {
          state = 1;
        }
      } else {
        t0 = millis();
      }
      break;

    case 1:  // headset komt op de juiste hoogte
      setColorRGB(255, 0, 0);
      dist = 0.0;

      for(int i = 0; i < 10; i++){
        double meas = distance();
        if ((ceilHeight - meas) >= 1000){ // metingen van objecten korter dan 1m worden niet meegeteld
          dist += meas;
        } else{
          i--;
        }
      }
      dist /= 10;
      dist *= 1.20;

      if (step((int) (-dist * STEPS_PER_MM), 800, HIGH)) {
        state = 6;  // error handling
        break;
      }

      Serial.println("s");  // game start commando (iets van safety toevoegen voor 100% zekere overdracht?)
      state = 2;
      fadeVal = 0;
      sinVal = 0.0;
      //fadeDir = HIGH;
      //t0 = millis();
      break;

    case 2:  //game is bezig, wacht op serial bus commando

      fadeVal = (byte) (127.5 + 127.5 * sin(sinVal));
      sinVal += 0.0002;

      setColorRGB(0, 0, fadeVal);

      if (Serial.available() > 0) {
        int input = (char)Serial.read();  // wachten op "e" in de Serial bus (stop signal)
        if (input == 'e') {
          state = 3;
        }
      }
      break;

    case 3:  //game is over, wachten tot user is afgestapt, dan bril optillen.
      zetBrilAf();
      stapAf();
      if (step(100, 800, HIGH)) {
        state = 6;  // error handling
        break;
      }
      delay(3000);
      calibrate();
      state = 0;
      break;

    case 6:  //bril tegen endstop gebotst, user van platform laten stappen, kleur flikkerend rood totdat user van het platform stapt. (index kan nog veranderen)
      stapAf();
      state = 0;
      break;
  }

  if(analogRead(pressurePin) > pressureTreshold || state == 0){
    t_reset = millis();
    t1 = millis();
  }


  if(t1 - t_reset >= resetTime){
    bool check = HIGH;
    for(int i = 0; i < 5; i++){
      double dist = distance();
      if (dist < treshold || dist >= ceilHeight){ // 5 consecutive height checks
        check = LOW;
        t_reset = millis();
        break;
      }
    }
    if(check){
      calibrate();
      state = 0;
      check = LOW;
    }
  }
}