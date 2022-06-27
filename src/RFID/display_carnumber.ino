/*
 * Initial Author: ryand1011 (https://github.com/ryand1011)
 *
 * Reads data written by a program such as "rfid_write_personal_data.ino"
 *
 * See: https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 *
 * Uses MIFARE RFID card using RFID-RC522 reader
 * Uses MFRC522 - Library
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
*/
#include <string.h>
#include <SPI.h>
#include <MFRC522.h>
#include<LiquidCrystal.h> //LCD 라이브러리를 추가
//LiquidCrystal lcd(RS, Enable, DB4, DB5, DB6, DB7);
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);


#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//*****************************************************************************************//
void setup() {
  Serial.begin(9600);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
}

//*****************************************************************************************//
void loop() {

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
  Serial.println();
  Serial.print("차량번호");
  Serial.println(str2);
  //Serial.println(strlen(str2));


  //----------------------------------------

  Serial.println(F("\n**End Reading**\n"));

  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();


  lcd.setCursor(5, 0);  //커서를 (5, 0)으로 보내라
  lcd.print(str2); //(5, 0)부터 Hello!!를 찍어라
  if (a == 1){
    lcd.setCursor(1,1);   //커서를 (1,1)로 옮겨라
    lcd.print("Go blue Light"); //(1,1)부터 Codingrun.com을 찍어라.
  }
}
//*****************************************************************************************//
