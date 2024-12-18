void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    int input = Serial.read();
    if(input == 49){
      digitalWrite(13, HIGH);
      Serial.write("Led aan");
    } else {
      digitalWrite(13, LOW);
      Serial.write("Led uit");
    }
  }
}
