#include<Adafruit_NeoPixel.h>

#define PIN 7  
#define PIN1 8  
#define NUMPIXELS 27 //소자 개수 파악
#define BRIGHTNESS 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800);
int trigPin = 2;
int echoPin = 3;
void setup(){ 
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(PIN, OUTPUT);
  pinMode(PIN1, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}
void loop(){
  digitalWrite(trigPin, LOW);
  delay(30);
  digitalWrite(trigPin, HIGH);
  delay(30);
  digitalWrite(trigPin, LOW); //초음파 보내고 받기
  
  float duration = pulseIn(echoPin, HIGH); //마이크로 초 구하기
  float distance = ((float)            00) / 2; //거리 구하기
  if(distance >= 2 && distance <= 3){
    colorWipe(strip.Color(0,0,100), strip1.Color(0,0,100), 0);
    Serial.print(distance);
    Serial.println("r\n");
    exit(1);
  }
  else if(distance > 4){
    colorWipe(strip.Color(100,100,0),strip1.Color(100,100,0),50);
    Serial.print(distance);
    Serial.println("l\n");
  }
  else if(distance < 2){
    colorReWipe(strip.Color(100,0,0),strip1.Color(100,0,0),50);
    Serial.print(distance);
    Serial.println("s\n");
  }
  delay(3000);
}
void colorWipe(uint32_t c, uint32_t c1, uint8_t wait){
  for(int i = 0; i < NUMPIXELS;i++){
    strip.setPixelColor(i,c);
    stripe.setPixelColor(i,c1);
    strip.show();
    stripe.show();
    delay(wait);
  }
}
void colorReWipe(uint32_t c, uint32_t c1, uint8_t wait){
  for(int i = NUMPIXELS-1;i>=0;i--){
    strip.setPixelColor(i,c);
    stripe.setPixelColor(i,c1);
    strip.show();
    stripe.show();
    delay(wait);
  }
}
