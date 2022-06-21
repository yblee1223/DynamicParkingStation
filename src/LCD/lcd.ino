#include<LiquidCrystal.h> //LCD 라이브러리를 추가
//LiquidCrystal lcd(RS, Enable, DB4, DB5, DB6, DB7);
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

void setup() {
  lcd.begin(16, 2); //16열 2행짜리 LCD를 사용하겠다.
}

void loop() {
  lcd.setCursor(5, 0);  //커서를 (5, 0)으로 보내라
  lcd.print("마이더부"); //(5, 0)부터 Hello!!를 찍어라
  lcd.setCursor(1,1);   //커서를 (1,1)로 옮겨라
  lcd.print("선형대수"); //(1,1)부터 Codingrun.com을 찍어라.
}
