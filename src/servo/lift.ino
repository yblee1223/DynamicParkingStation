#include <Servo.h>

Servo myServo;      
int potPin = A0;   
int potVal;        
int angle;         

void setup() {
  myServo.attach(9);
}

void loop() {
  
  potVal = analogRead(potPin); 
  
  angle = map(potVal, 0, 1023, 0, 179);
  
  myServo.write(angle);
  
  delay(15);
}
