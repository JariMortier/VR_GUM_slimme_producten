//rgb
const uint8_t redPin = 11;
const uint8_t greenPin = 10;
const uint8_t bluePin = 9;

double hue = 0.0;

void setColorRGB(int red, int green, int blue){
  /*
  red = red % 255;
  green = green % 255;
  blue = blue % 255;
  */
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


void setup() {
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  /*
  setColorHSL(hue, 1, 0.5);
  hue += 0.002;
  if(hue > 1){
    hue -= 1;
  }
  delay(10);*/
  
  if(Serial.available() > 0){
    int input = Serial.read();
    switch(input){
      case 114:
        setColorRGB(255, 0, 0);
        Serial.println("red selected");
        break;

      case 103:
        setColorRGB(0, 255, 0);
        Serial.println("green selected");
        break;

      case 98:
        setColorRGB(0, 0, 255);
        Serial.println("blue selected");
        break;

      default:
        setColorRGB(0, 0, 0);
        Serial.println("no color selected");
        break;
    }
    delay(1000);
  }
  
}