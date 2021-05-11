const int trigPin = 9;
const int echoPin = 10;
const int lightOnOff = 2;
//for pid leds
//4 is 1, 7 is 2, 8 is 3, 12 is 4
const int lightPID1 = 4;
const int lightPID2 = 7;
const int lightPID3 = 8;
const int lightPID4 = 12;
long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT); //d1 is echo, d0 is trig
  pinMode(echoPin, INPUT); //echo is input, trigger is output
  pinMode(lightOnOff, INPUT); //echo is input, trigger is output
  Serial.begin(115200);
  digitalWrite(lightOnOff, LOW);
  digitalWrite(lightPID1, LOW);
  digitalWrite(lightPID2, LOW);
  digitalWrite(lightPID3, LOW);
  digitalWrite(lightPID4, LOW);

}
void checkdistanceOnOff() {
  if(distance <10){
    digitalWrite(lightOnOff, LOW);
    delay(500);
  }
}
void checkdistancePID() {
  if(distance>50){
    digitalWrite(lightPID4, HIGH);
    digitalWrite(lightPID3, HIGH);
    digitalWrite(lightPID2, HIGH);
    digitalWrite(lightPID1, HIGH);
  }
  if(distance <50){
    digitalWrite(lightPID4, LOW);
    digitalWrite(lightPID3, HIGH);
    digitalWrite(lightPID2, HIGH);
    digitalWrite(lightPID1, HIGH);
    delay(500);
  }
  if(distance <40){
    digitalWrite(lightPID4, LOW);
    digitalWrite(lightPID3, LOW);
    digitalWrite(lightPID2, HIGH);
    digitalWrite(lightPID1, HIGH);
    delay(500);
  }
  if(distance <30){
    digitalWrite(lightPID4, LOW);
    digitalWrite(lightPID3, LOW);
    digitalWrite(lightPID2, LOW);
    digitalWrite(lightPID1, HIGH);
    delay(500);
  }
  if(distance <20){
    digitalWrite(lightPID4, LOW);
    digitalWrite(lightPID3, LOW);
    digitalWrite(lightPID2, LOW);
    digitalWrite(lightPID1, LOW);
    delay(500);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  digitalWrite(lightOnOff, HIGH);
  /*digitalWrite(lightPID1, HIGH);
  digitalWrite(lightPID2, HIGH);
  digitalWrite(lightPID3, HIGH);
  digitalWrite(lightPID4, HIGH);*/


  duration = pulseIn(echoPin, HIGH); //return length of pulse in microseconds
  distance = duration*0.034/2; //multiply by speed of sound, divide by 2 cause of two trips    

  Serial.print("Distance: ");
  Serial.println(distance);
  checkdistanceOnOff();
  checkdistancePID();
  
  //Serial.print(duration);

}
