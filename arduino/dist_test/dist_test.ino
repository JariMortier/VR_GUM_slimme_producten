/*
#define echoPin 6 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5 // attach pin D3 Arduino to pin Trig of HC-SR04


long duration; // Variable to store time taken to the pulse
               // to reach receiver

float distance; // Variable to store distance calculated using
              // formula

void setup()
{
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

    // Serial Communication is starting with 9600 of
    // baudrate speed
    Serial.begin(9600);

    // The text to be printed in serial monitor
    Serial.println("Distance measurement using Arduino Uno.");
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // wait for 2 ms to avoid
                        // collision in serial monitor

  digitalWrite(trigPin, HIGH); // turn on the Trigger to generate pulse
  delayMicroseconds(10); // keep the trigger "ON" for 10 µs to generate
            // pulse for 10 µs.

  digitalWrite(trigPin, LOW); // Turn off the pulse trigger to stop
                      // pulse generation

  // If pulse reached the receiver echoPin
  // become high Then pulseIn() returns the
  // time taken by the pulse to reach the
  // receiver

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.172; // Expression to calculate distance using time

  Serial.println(distance);
}*/
//---------------------------------------------------------------------------------------------------------------
const int trig_pin = 5;
const int echo_pin = 6;

float timing = 0.0;
//float distance = 0.0;

float distance(){ //takes about 200µs
  float distance = 0;
  long duration = 0;
  for(int i = 0; i < 5; i++){

    digitalWrite(trig_pin, LOW);
    delay(2);
    
    digitalWrite(trig_pin, HIGH);
    delay(10);
    digitalWrite(trig_pin, LOW);
    
    duration = pulseIn(echo_pin, HIGH);
    distance += (duration * 0.034) / 2;
  }
  return distance*2;
}

void setup()
{
  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  
  digitalWrite(trig_pin, LOW);

  Serial.begin(9600);
}

void loop()
{
  /*
  digitalWrite(trig_pin, LOW);
  delay(2);
  
  digitalWrite(trig_pin, HIGH);
  delay(10);
  digitalWrite(trig_pin, LOW);
  
  timing = pulseIn(echo_pin, HIGH);
  distance = (timing * 0.034) / 2;
  */
  //Serial.print("Distance: ");
  Serial.println(distance());
  //Serial.println("mm");
  
  delay(100);
}
