#include <string.h>
// LED
#include <Adafruit_NeoPixel.h> // led
// SONIC
#define BRIGHTNESS 100
// ACT
#include <Servo.h>

Servo myservo; 
// SONIC

#define MAXPIN 9 
#define NUMPIXELS 27
  

// struct
typedef struct sector { // parking sector
    int sector_num;

    // CDS
    int car_in;
    
    // LED
    int width[2];
    int height[2];
    // 0 -> start
    // 1 -> finish
    int i_hat[2]; // 0, 1
    int j_hat[2]; // 4, 5
    // up -> down
    // right -> left
}Sector;

// init
int plus_pps=0;
Sector s[8];

// LED
int pin_num[9] = {4, 5, 6, 7, 8, 9, 10, 11, 12}; // used pin NeoPIXEL
int sw[4] = {51, 52, 53, 47}; // used pin switch
Adafruit_NeoPixel selector = Adafruit_NeoPixel(NUMPIXELS, pin_num[0], NEO_GRB + NEO_KHZ800);
// 가로 0, 1, 2, 3
// 세로 4, 5, 6, 7, 8 

// CDS
int cds[8] = {A0, A1, A2, A3, A4, A5, A6, A7}; // analogpin
// ACT
const int SWITCH_A = 48;
const int SWITCH_B = 49;
int pos = 0;    // variable to store the servo position

// SONIC
int trigPin = 2;
int echoPin = 3;
Adafruit_NeoPixel strip;
Adafruit_NeoPixel strip1;

// Servo

/* --- init func --- */
void init_sector();
void init_led();
void init_sonic();



// main
void led_main();
void sonic_main();
void cds_main();

//dps
void find_parking_place();
void pps();

//sonic
void colorWipe1(uint32_t c, uint32_t c1, uint8_t wait);
void colorReWipe(uint32_t c, uint32_t c1, uint8_t wait);
void hps(int distance);

// cds
void cds_state();

//led
void select(int n);

// bright
void bright_H();
void bright_playground();
void bright_all();
void bright_off();
void bright_allsector();
void bright_sector(int sector_num, char color);
void bright_7segment(int sum);// 가운데 걸로 7segment 주차 대수 보여주기
void bright_7segment(int sum);
void count_parking();

void setup() 
{
  Serial.begin(9600);
  init_sector();
  init_led();
  init_sonic();
  //init_servo();
}
 
void loop() 
{
  dps_main();
}

/* ---- func ---- */

// init


void init_sector()
{
    // init up & down
    int width_start[4] = { 0, 5, 10, 15 };
    int width_finish[4] = { 6, 10, 15, 20 };

    // init left & right
    int height_start[2] = { 18, 1 };
    int height_finish[2] = { 27, 8 };

    // init basis
    int led_line_i_hat[4] = { 0, 1, 2, 3 };
    int led_line_j_hat[5] = { 4, 5, 6, 7, 8 };

    // init sector_num
    for (int i = 0; i < 8; i++) {
        // sector_num
        s[i].sector_num = i;

        // CDS
        s[i].car_in = 0;

        // LED
        // start & finish
        s[i].width[0] = width_start[i % 4];
        s[i].width[1] = width_finish[i % 4];
        s[i].height[0] = height_start[i / 4];
        s[i].height[1] = height_finish[i / 4];

        // up& down
        s[i].i_hat[0] = led_line_i_hat[i / 4 * 2];
        s[i].i_hat[1] = led_line_i_hat[i / 4 * 2 + 1];
        // left & right
        s[i].j_hat[0] = led_line_j_hat[i % 4];
        s[i].j_hat[1] = led_line_j_hat[i % 4 + 1];
    }
}
void init_servo()
{
  myservo.attach(50);
  pinMode(SWITCH_B, INPUT);
  pinMode(SWITCH_A, INPUT);
}
void init_led()
{
  // pinMode

  for (int i = 0; i < 9; i++){
    pinMode(pin_num[i], OUTPUT);
  }
  // This initializes the NeoPixel library.
  
  pinMode(sw[0], INPUT_PULLUP);
  pinMode(sw[1], INPUT_PULLUP);
  pinMode(sw[2], INPUT_PULLUP);
  pinMode(sw[3], INPUT_PULLUP);
}

void init_sonic()
{
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

// DPS
void dps_main()
{
  // servo
  // lift();
  
  // CDS
  cds_main();
 
  // SONIC
  sonic_main();
  
  // LED
  led_main();
  
  //lift();
}


// SONIC
//*****************************************************************************************//
void sonic_main()
{
  digitalWrite(trigPin, LOW);
  delay(30);
  digitalWrite(trigPin, HIGH);
  delay(30);
  digitalWrite(trigPin, LOW); //초음파 보내고 받기
  float duration = pulseIn(echoPin, HIGH); //마이크로 초 구하기
  float distance = ((340 * duration) / 10000)/ 2; //거리 구하기
  //float distance = duration*0.17;
  Serial.println(distance);
  
  if (distance < 14.5){
    strip = Adafruit_NeoPixel(NUMPIXELS, pin_num[7], NEO_GRB + NEO_KHZ800);
    strip1 = Adafruit_NeoPixel(NUMPIXELS, pin_num[8], NEO_GRB + NEO_KHZ800);
    hps(distance);
  }
  
}

void hps(float distance)// 주차 보조 시스템
{ 
  bright_off();
  while(distance < 14){
    strip.begin();
    strip1.begin();
    digitalWrite(trigPin, LOW);
    delay(30);
    digitalWrite(trigPin, HIGH);
    delay(30);
    digitalWrite(trigPin, LOW); //초음파 보내고 받기
    float duration = pulseIn(echoPin, HIGH); //마이크로 초 구하기
    float distance = ((340 * duration) / 10000)/ 2; //거리 구하기
    //float distance = duration*0.17;
    Serial.println(distance);
  
  if(distance >= 1.5 && distance <= 2){
    colorWipe1(strip.Color(0,0,100), strip1.Color(0,0,100), 0);
    Serial.print(distance);
    Serial.println("r\n");
    bright_off();
    delay(3000);
    return;
  }
  else if(distance > 1.5){
    colorWipe1(strip.Color(100,100,0),strip1.Color(100,100,0),50);
    Serial.print(distance);
    Serial.println("l\n");
  }
  else if(distance < 2){
    colorReWipe(strip.Color(100,0,0),strip1.Color(100,0,0),50);
    Serial.print(distance);
    Serial.println("s\n");
  }
  //delay(3000);
  }
}
void colorWipe1(uint32_t c, uint32_t c1, uint8_t wait){
  for(int i = 18; i < 27;i++){
    strip.setPixelColor(i,c);
    strip1.setPixelColor(i,c1);
    strip.show();
    strip1.show();
    delay(wait);
  }
}
void colorReWipe(uint32_t c, uint32_t c1, uint8_t wait){
  for(int i = 27 - 1; i >= 18; i--){
    strip.setPixelColor(i,c);
    strip1.setPixelColor(i,c1);
    strip.show();
    strip1.show();
    delay(wait);
  }
}

//Servo
void lift()
{
   if (digitalRead(SWITCH_A)==HIGH){
    Serial.println(digitalRead(SWITCH_A));
    for (pos = 0; pos <= 5; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
      myservo.write(pos); // tell servo to go to position in variable 'pos'
      delay(50);
                   
    }
  }

  if (digitalRead(SWITCH_B)==HIGH){
    Serial.println(digitalRead(SWITCH_B));
    for (pos = 10; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(50);
    }
  }

}

// CDS
//*****************************************************************************************//
void cds_main()
{
  cds_state();
  find_parking_place();
  pps();
}
void cds_state()
{
  int cdsValue;
  for (int i = 0; i< 8; i++){
    cdsValue = analogRead(cds[i]);
    if (cdsValue > 220){
      //digitalWrite(led, HIGH);
      s[i].car_in = 1;
      //bright_off();
      //bright_sector(i, 'B');
      // cds에 밝기가 감지되면 bright sector
    }
    else{
      s[i].car_in = 0;
    }
  }
}

void find_parking_place()// 차량이 처음 출입시 주차할 장소를 찾는다.
{
  for (int i = 0; i < 8; i++){
     if (s[i].car_in == 0){
      bright_off();
      bright_sector(i, 'G');
      delay(1000);
      break;
     }
     else{
      bright_off();
     }
  }
}
void pps()//장애인 구역 보존 알고리즘
{ if (plus_pps == 0){
  if (s[0].car_in == 1 && s[1].car_in == 1){
    for (int i = 2; i < 8; i++){
      if (s[i].car_in == 0){
        plus_pps=1;
        bright_sector(i, 'B');// blue
        break;
      }
    }
  }
  }
}
void count_parking(){
  int sum = 0;
  for (int i; i < 8; i++){
    sum += s[i].car_in;
  }
  bright_7segment(sum);
}











// LED
//*****************************************************************************************//
void select(int n)
{
  selector = Adafruit_NeoPixel(NUMPIXELS, pin_num[n], NEO_GRB + NEO_KHZ800);
  selector.begin();
  //selector.show();
}


void led_main()
{  
  if(digitalRead(sw[0]) == LOW){ // 
    bright_off();
    Serial.println(digitalRead(sw[0]));
    bright_playground();
    delay(3000);
  }
  else if(digitalRead(sw[1]) == LOW){
    bright_off();
    Serial.println(digitalRead(sw[1]));
    bright_all();
    delay(3000);
  }
  else if(digitalRead(sw[2]) == LOW){
    bright_off();
    Serial.println(digitalRead(sw[2]));
    bright_H();
    delay(3000);
  }
  else if(digitalRead(sw[3]) == LOW){
    bright_off();
    Serial.println(digitalRead(sw[3]));
    bright_7segment();
    delay(3000);
  }
  bright_off();
}

// bright
void bright_off()
{
  for(int i = 0; i < 9; i++){
    select(i);
    colorWipe(selector.Color(0,0,0), 0, i);
  }
}
void bright_playground()
{
  // play ground
  select(0);
  colorWipe(selector.Color(0,100,0), 0, 0);
  select(1);
  colorWipe(selector.Color(0,100,0), 0, 1);
  select(2);
  colorWipe(selector.Color(0,100,0), 0, 2);
  select(3);
  colorWipe(selector.Color(0,100,0), 0, 3);
  select(4);
  colorWipe(selector.Color(0,100,0), 0, 4);
  select(6);
  colorWipe(selector.Color(0,100,0), 0, 6);
  select(8);
  colorWipe(selector.Color(0,100,0), 0, 8);
  
}

void bright_H()
{
  // H
  select(0);
  colorWipe(selector.Color(100,0,0), 0, 0);
  select(3);
  colorWipe(selector.Color(100,0,0), 0, 3);
  select(4);
  colorWipe(selector.Color(100,0,0), 0, 4);
  select(8);
  colorWipe(selector.Color(100,0,0), 0, 8);
  select(1);
  colorWipeScope(selector.Color(100,0,0), 0, 1, 5, 15);
  select(6);
  colorWipeScope(selector.Color(100,0,0), 0, 6, 8, 18);
  select(2);
  colorWipeScope(selector.Color(100,0,0), 0, 2, 5, 15);
  //colorWipeScope(selector.Color(100,0,0), 0, 6, 8, 18);
  
}

void bright_sector(int sector_num, char color)
{
  int c[3] = {0, 0, 100};// default
  // select color
  if (color == 'R'){
    int c[3] = {100, 0, 0};
  }
  else if(color == 'G'){
    int c[3] = {0, 100, 0};
  }
  else if(color == 'B'){
    int c[3] = {0, 0, 100};
  }
  
  // up & down
  select(s[sector_num].i_hat[0]);
  colorWipeScope(selector.Color(c[0],c[1],c[2]), 0, s[sector_num].i_hat[0], s[sector_num].width[0], s[sector_num].width[1]);
  select(s[sector_num].i_hat[1]);
  colorWipeScope(selector.Color(c[0],c[1],c[2]), 0, s[sector_num].i_hat[1], s[sector_num].width[0], s[sector_num].width[1]);
  

  // left & right
  select(s[sector_num].j_hat[0]);
  colorWipeScope(selector.Color(c[0],c[1],c[2]), 0, s[sector_num].j_hat[0], s[sector_num].height[0], s[sector_num].height[1]);
  select(s[sector_num].j_hat[0]);
  colorWipeScope(selector.Color(c[0],c[1],c[2]), 0, s[sector_num].j_hat[1], s[sector_num].height[0], s[sector_num].height[1]);
}
void bright_allsector()
{
  for (int i = 0; i < 8; i++){
     bright_sector(i, 'R');
  }
}

void bright_all()
{
  for(int i = 0; i < 9; i++){
    select(i);
    colorWipe(selector.Color(100,100,100), 0, i);
  }
}


void bright_7segment(int sum){
  int digitForNum[10][8] = {
  {1, 1, 1, 1, 1, 1, 0, 1}, //0
  {0, 1, 1, 0, 0, 0, 0, 1}, //1
  {1, 1, 0, 1, 1, 0, 1, 1}, //2
  {1, 1, 1, 1, 0, 0, 1, 1}, //3
  {0, 1, 1, 0, 0, 1, 1, 1}, //4
  {1, 0, 1, 1, 0, 1, 1, 1}, //5
  {1, 0, 1, 1, 1, 1, 1, 1}, //6
  {1, 1, 1, 0, 0, 0, 0, 1}, //7
  {1, 1, 1, 1, 1, 1, 1, 1}, //8
  {1, 1, 1, 1, 0, 1, 1, 1}  //9
  };
  
  bright_off();
  
  for (int i = 0; i < 9; i++){
    if (digitForNum[sum][i] == 1){
      bright_element(i);
    }
  }
  delay(5000);
  bright_off();
    
}
void bright_elements(int num){ // for 7_segment
  switch(num){
    case 0:
      // a
      select(8);
      colorWipeScope(selector.Color(0,100,0), 0, 8, 8, 18);
    case 1:
      // b
      select(2);
      colorWipeScope(selector.Color(0,100,0), 0, 2, 0, 10);
    case 2:
      // c
      select(2);
      colorWipeScope(selector.Color(0,100,0), 0, 2, 10, 20);
    case 3:
      // d
      select(4);
      colorWipeScope(selector.Color(0,100,0), 0, 4, 8, 18);
    case 4:
      // e
      select(1);
      colorWipeScope(selector.Color(0,100,0), 0, 1, 10, 20);
    case 5:
      // f
      select(1);
      colorWipeScope(selector.Color(0,100,0), 0, 1, 0, 10);
    case 6:
      // g
      select(6);
      colorWipeScope(selector.Color(0,100,0), 0, 6, 8, 18);
  }
}

// color wipe
void colorWipe(uint32_t c, uint16_t wait, int n)
{
  select(n);
  for (int i = 0; i < NUMPIXELS; i++){
    selector.setPixelColor(i,c);
    selector.show();
    delay(wait);
  }
}
void colorWipeScope(uint32_t c, uint16_t wait, int n, int start, int finish)
{
  select(n);
  for (int i = start; i < finish; i++){
    selector.setPixelColor(i,c);
    selector.show();
    delay(wait);
  }
}
