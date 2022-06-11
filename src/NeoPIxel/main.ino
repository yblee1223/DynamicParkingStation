#include <Adafruit_NeoPixel.h> // led

// led 
#define MAXPIN 9 
#define NUMPIXELS 27
  

//led variable
typedef struct sector { // parking sector
    int sector_num;
    
    int width[2];
    int height[2];
    // 0 -> start
    // 1 -> finish

    int i_hat[2]; // 0, 1
    int j_hat[2]; // 4, 5
    // up -> down
    // right -> left
}Sector;

Sector s[8];

int pin_num[9] = {2, 3, 12, 13, 4, 5, 6, 7, 8}; // used pin NeoPIXEL
int sw[3] = {9, 10, 11}; // used pin switch
Adafruit_NeoPixel selector = Adafruit_NeoPixel(NUMPIXELS, pin_num[0], NEO_GRB + NEO_KHZ800);


// 가로 0, 1, 2, 3
// 세로 4, 5, 6, 7, 8 


/* --- init func --- */
void init_led();
void init_sector();
void led_main();
void select(int n);
void bright_off();

// bright
void bright_H();
void bright_playground();
void bright_all();
void bright_allsector();
void bright_sector();





void setup() 
{
  Serial.begin(9600);
  init_led();
  init_sector();
  //bright_H();
  //bright_playground();
}
 
void loop() 
{
  //select(2);
  //colorWipe(selector.Color(100,0,0), 0, 1);
  bright_H();
  //bright_off();
  //delay(3000);
  //bright_sector(6);
  delay(3000);
}

/* ---- func ---- */

// init
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
void select(int n)
{
  selector = Adafruit_NeoPixel(NUMPIXELS, pin_num[n], NEO_GRB + NEO_KHZ800);
  selector.begin();
  //selector.show();
}
void bright_off()
{
  for(int i = 0; i < 9; i++){
    select(i);
    colorWipe(selector.Color(0,0,0), 0, i);
  }
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
  //colorWipe(pixels.Color(0,0,0),100);
}

// bright

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

void bright_sector(int sector_num)
{
  // up & down
  select(s[sector_num].i_hat[0]);
  colorWipeScope(selector.Color(0,0,100), 0, s[sector_num].i_hat[0], s[sector_num].width[0], s[sector_num].width[1]);
  select(s[sector_num].i_hat[1]);
  colorWipeScope(selector.Color(0,0,100), 0, s[sector_num].i_hat[1], s[sector_num].width[0], s[sector_num].width[1]);
  

  // left & right
  select(s[sector_num].j_hat[0]);
  colorWipeScope(selector.Color(0,0,100), 0, s[sector_num].j_hat[0], s[sector_num].height[0], s[sector_num].height[1]);
  select(s[sector_num].j_hat[0]);
  colorWipeScope(selector.Color(0,0,100), 0, s[sector_num].j_hat[1], s[sector_num].height[0], s[sector_num].height[1]);
}
void bright_allsector()
{
  for (int i = 0; i < 8; i++){
     bright_sector(i);
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

