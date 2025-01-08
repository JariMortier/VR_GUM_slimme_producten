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
#define pressurePin A0

//rgb
const uint8_t redPin = 11;
const uint8_t greenPin = 10;
const uint8_t bluePin = 9;

int dist = 0;
int state = 0;

byte fadeVal = 0;
bool fadeDir = HIGH;
double hue = 0.0;

//timing
uint32_t t0 = 0;
uint32_t t1 = 0;

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

void stapAf(){
  t0 = millis();
  bool flashState = LOW;
  while (analogRead(pressurePin) > 350) {
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

void setup() {
  Serial.begin(9600);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(limitPin, INPUT);
  pinMode(pressurePin, INPUT);
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

      if (analogRead(pressurePin) > 350) {
        if (t1 - t0 >= 2000) {
          state = 1;
        }
      } else {
        t0 = millis();
      }
      break;

    case 1:  // headset komt op de juiste hoogte
      setColorRGB(255, 0, 0);
      while (digitalRead(limitPin)) {
      }

      Serial.println("s");  // game start commando (iets van safety toevoegen voor 100% zekere overdracht?)
      state = 2;
      fadeVal = 0;
      fadeDir = HIGH;
      t0 = millis();
      break;

    case 2:  //game is bezig, wacht op serial bus commando
      if (t1 - t0 >= 10) {
        t0 = millis();
        if (fadeDir) {
          fadeVal++;
        } else {
          fadeVal--;
        }
        if (fadeVal == 255) {
          fadeDir = LOW;
        } else if (fadeVal == 0){
          fadeDir = HIGH;
        }
      }
      setColorRGB(0, 0, fadeVal);

      if(Serial.available() > 0){
        int input = (char) Serial.read(); // wachten op "e" in de Serial bus (stop signal)
        if (input == 'e'){
          state = 3;
          input = 0;
        }
      }
      break;

    case 3: //game is over, wachten tot user is afgestapt, dan bril optillen.
      stapAf();
      // meten ofdat bril is afgezet!!!
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
