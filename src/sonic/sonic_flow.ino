#include <string.h>
// LED
#include <Adafruit_NeoPixel.h> // led
// RFID
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         21          // Configurable, see typical pin layout above
#define SS_PIN          17          // Configurable, see typical pin layout above
// SONIC
#define BRIGHTNESS 100
// LCD
#include<LiquidCrystal.h> //LCD 라이브러리

char car_num[100];
int D; // disable
int W;// wheel chair
// ACT
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
Sector s[8];

// LED
int pin_num[9] = {4, 5, 6, 7, 8, 9, 10, 11, 12}; // used pin NeoPIXEL
int sw[3] = {9, 10, 11}; // used pin switch
Adafruit_NeoPixel selector = Adafruit_NeoPixel(NUMPIXELS, pin_num[0], NEO_GRB + NEO_KHZ800);
// 가로 0, 1, 2, 3
// 세로 4, 5, 6, 7, 8 

// CDS
int cds[8] = {A0, A1, A2, A3, A4, A5, A6, A7}; // analogpin
// RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
// LCD
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);
// ACT
// SONIC
int trigPin = 15;
int echoPin = 14;
Adafruit_NeoPixel strip;
Adafruit_NeoPixel strip1;


/* --- init func --- */
void init_sector();
void init_led();
void init_rfid();
void init_sonic();



// main
void led_main();
void lcd_main();
void sonic_main();
void rfid_main();
void cds_main();

//dps
void find_parking_place();
void pps();

//sonic
void colorWipe1(uint32_t c, uint32_t c1, uint8_t wait);
void colorReWipe(uint32_t c, uint32_t c1, uint8_t wait);
void hps(int distance);
//lcd

//rfid
void read_data();

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
void bright_7_segment();// 가운데 걸로 7segment 주차 대수 보여주기


void setup() 
{
  Serial.begin(9600);
  init_sector();
  init_led();
  init_rfid();
  lcd.begin(16, 2); // LCD
  //bright_H();
  //bright_playground();
}
 
void loop() 
{
  // CDS
  cds_main();
  
  // LED
  led_main();
  
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
void init_rfid()
{
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
}

void init_led()
{
  // pinMode

  for (int i = 0; i < 9; i++){
    pinMode(pin_num[i], OUTPUT);
  }
  // This initializes the NeoPixel library.
  
  pinMode(sw[0], INPUT);
  pinMode(sw[1], INPUT);
  pinMode(sw[2], INPUT);
  bright_off();
}

void init_sonic()
{
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}


void dps_main()
{
  // RFID
  // if disabled
  // if 
  // if parking
  // 
  // GATE
  // CDS
  // LED
  rfid_main();
  cds_main();
  led_main();
}
// DPS
// LCD
void lcd_main()
{
  lcd.setCursor(5, 0);  //커서를 (5, 0)으로 보내라
  lcd.print("마이더부"); //(5, 0)부터 Hello!!를 찍어라
  lcd.setCursor(1,1);   //커서를 (1,1)로 옮겨라
  lcd.print("선형대수"); //(1,1)부터 Codingrun.com을 찍어라
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
  float distance = ((float)            00) / 2; //거리 구하기

  if (distance < 7){
    strip = Adafruit_NeoPixel(NUMPIXELS, pin_num[4], NEO_GRB + NEO_KHZ800);
    strip1 = Adafruit_NeoPixel(NUMPIXELS, pin_num[5], NEO_GRB + NEO_KHZ800);
    hps(distance);
  }
}

void hps(int distance)// 주차 보조 시스템
{ 
  while(distance > 2){
  if(distance >= 2 && distance <= 3){
    colorWipe1(strip.Color(0,0,100), strip1.Color(0,0,100), 0);
    Serial.print(distance);
    Serial.println("r\n");
    exit(1);
  }
  else if(distance > 4){
    colorWipe1(strip.Color(100,100,0),strip1.Color(100,100,0),50);
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
}
void colorWipe1(uint32_t c, uint32_t c1, uint8_t wait){
  for(int i = 0; i < NUMPIXELS;i+=2){
    strip.setPixelColor(i,c);
    strip1.setPixelColor(i,c1);
    strip.show();
    strip1.show();
    delay(wait);
  }
}
void colorReWipe(uint32_t c, uint32_t c1, uint8_t wait){
  for(int i = NUMPIXELS - 1; i>=0; i-=2){
    strip.setPixelColor(i,c);
    strip1.setPixelColor(i,c1);
    strip.show();
    strip1.show();
    delay(wait);
  }
}
// RFID
//*****************************************************************************************//
void rfid_main()
{
  read_data();
}
void read_data()
{
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

  //-------------------------------------------
  int cnt = 0;
  

  byte buffer1[18];
  char str1[100] = " "; // 2진수 입력
  int a = 0, b = 0;
  block = 1;
  len = 18;

  //------------------------------------------- GET binary code
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  Serial.print(F("data 1, data 2: "));
  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
      if (buffer1[i] == ' ')
        break;
      str1[i] = buffer1[i];
    }
  }
  Serial.println();
  a = str1[0]-'0';
  b = str1[1]-'0';
  D = a;
  W = b;
  Serial.print("장애인 차량: ");
  Serial.println(a);
  Serial.print("휠체어 유무: ");
  Serial.println(b);
  //Serial.println(str1);
  //Serial.println(strlen(str1));
  
  Serial.println(" ");
  
  //---------------------------------------- GET car number

  byte buffer2[18];
  char str2[100] = " "; // 차량번호
  block = 4;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  cnt = 0;
  Serial.print(F("data 3: "));
  //PRINT LAST NAME
  for (uint8_t i = 1; i < 16; i++) {
    Serial.write(buffer2[i] );
    if (buffer2[i] == ' '){
      if (cnt == 1)
        break;
      cnt = 1;
    }
    str2[i-1] = buffer2[i];
  }
  strcpy(car_num, str2);
  //Serial.println();
  //Serial.print("차량번호");
  //Serial.println(str2);
  //Serial.println(strlen(str2));


  //----------------------------------------

  Serial.println(F("\n**End Reading**\n"));

  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}


// CDS
//*****************************************************************************************//
void cds_main()
{
  cds_state();
  //find_parking_place();
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
      bright_off();
      bright_sector(i, 'B');
      // cds에 밝기가 감지되면 bright sector
    }
    else{
      s[i].car_in = 0;
    }
  }
}

void find_parking_place()// 차량이 처음 출입시 주차할 장소를 찾는다.
{
  // if tagging
  for (int i = 0; i < 8; i++){
     if (s[i].car_in == 0){
      bright_sector(i, 'G');
      break;
     }
  }
}
void pps()//장애인 구역 보존 알고리즘
{
  if (s[0].car_in == 1 && s[1].car_in == 1){
    for (int i = 2; i < 8; i++){
      if (s[i].car_in == 0){
        bright_sector(i, 'B');// blue
        break;
      }
    }
  }
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
  if(digitalRead(sw[0]) == HIGH){ // 
    bright_playground();
    delay(500);
  }
  else if(digitalRead(sw[1]) == HIGH){
    bright_all();
    delay(500);
  }
  else if(digitalRead(sw[2]) == HIGH){
    bright_H();
    delay(500);
  }
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
  select(1);
  colorWipe(selector.Color(0,100,0), 0, 1);
  select(4);
  colorWipeScope(selector.Color(0,100,0), 0, 4, 8, 18);
  select(6);
  colorWipeScope(selector.Color(0,100,0), 0, 6, 8, 18);
  select(8);
  colorWipeScope(selector.Color(0,100,0), 0, 8, 8, 18);
  select(2);
  colorWipe(selector.Color(0,100,0), 0, 2);
}

void bright_H()
{
  // H
  select(1);
  colorWipe(selector.Color(100,0,0), 0, 1);
  select(6);
  colorWipeScope(selector.Color(100,0,0), 0, 6, 8, 18);
  select(2);
  colorWipe(selector.Color(100,0,0), 0, 2);
  
}

void bright_sector(int sector_num, char color)
{
  int c[3] = {100, 0, 0};// default
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
    colorWipe(selector.Color(100,0,0), 0, i);
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
