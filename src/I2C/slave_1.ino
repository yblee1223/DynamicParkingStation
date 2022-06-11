// slave
#include <Wire.h>

void setup() {
  Wire.begin(1); //슬레이브 주소                
  Wire.onReceive(receiveEvent); //데이터 전송 받을 때 receiveEvent함수 호출

  Serial.begin(9600);           
}

void loop() {
  delay(500);
}

void receiveEvent(int howMany) { //전송 데이터 읽기
  while (Wire.available()>1) { 
    char ch = Wire.read(); 
    Serial.print(ch);         
  }
  int x = Wire.read();    
  Serial.println(x);      
}
