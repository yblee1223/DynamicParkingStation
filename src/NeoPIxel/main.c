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
Adafruit_NeoPixel p0 = Adafruit_NeoPixel(NUMPIXELS, pin_num[0], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel p1 = Adafruit_NeoPixel(NUMPIXELS, pin_num[1], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel p2 = Adafruit_NeoPixel(NUMPIXELS, pin_num[2], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel p3 = Adafruit_NeoPixel(NUMPIXELS, pin_num[3], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel p4 = Adafruit_NeoPixel(NUMPIXELS, pin_num[4], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel p5 = Adafruit_NeoPixel(NUMPIXELS, pin_num[5], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel p6 = Adafruit_NeoPixel(NUMPIXELS, pin_num[6], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel p7 = Adafruit_NeoPixel(NUMPIXELS, pin_num[7], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel p8 = Adafruit_NeoPixel(NUMPIXELS, pin_num[8], NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel selector = Adafruit_NeoPixel(NUMPIXELS, pin_num[0], NEO_GRB + NEO_KHZ800);


// 가로 0, 1, 2, 3
// 세로 4, 5, 6, 7, 8 


/* --- init func --- */
void init_led();
void init_sector();
void led_main();

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
  bright_sector(7);
}
 
void loop() {
  //led_main();
  //Serial.println(digitalRead(sw[0]));
  //bright_all();
  //bright_sector(7);  
  //colorWipe(p[0].Color(0,0,100), 0, 1);
  p[8].setPixelColor(3, p[8].Color(0,0,100));
  
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
  p0.begin();
  p1.begin();
  p2.begin();
  p3.begin();
  p4.begin();
  p5.begin();
  p6.begin();
  p7.begin();
  p8.begin();
  
  // show
  p0.show();
  p1.show();
  p2.show();
  p3.show();
  p4.show();
  p5.show();
  p6.show();
  p7.show();
  p8.show();
  
  pinMode(sw[0], INPUT);
  pinMode(sw[1], INPUT);
  pinMode(sw[2], INPUT);
}

void init_sector()
{
    // init up & down
    int width_start[4] = { 1, 6, 11, 16 };
    int width_finish[4] = { 6, 11, 15, 20 };

    // init left & right
    int height_start[2] = { 1, 20 };
    int height_finish[2] = { 8, 27 };

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
  colorWipe(p1.Color(100,0,0), 0, 1);
  colorWipeScope(p4.Color(100,0,0), 0, 4, 8, 20);
  colorWipeScope(p6.Color(100,0,0), 0, 6, 8, 20);
  colorWipeScope(p8.Color(100,0,0), 0, 8, 8, 20);
  colorWipe(p2.Color(100,0,0), 0, 2);
}
void bright_H()
{
  // H
  colorWipe(p1.Color(100,0,0), 0, 1);
  colorWipeScope(p5.Color(100,0,0), 0, 5, 8, 20);
  colorWipe(p2.Color(100,0,0), 0, 2);
  
}
/*
void bright_sector(int sector_num)
{
  // up & down
  colorWipeScope(p[s[sector_num].i_hat[0]].Color(0,100,0), 0, s[sector_num].i_hat[0], s[sector_num].width[0], s[sector_num].width[1]);
  colorWipeScope(p[s[sector_num].i_hat[1]].Color(100,0,0), 0, s[sector_num].i_hat[1], s[sector_num].width[0], s[sector_num].width[1]);
  

  // left & right
  colorWipeScope(p[s[sector_num].j_hat[0]].Color(100,0,0), 0, s[sector_num].j_hat[0], s[sector_num].height[0], s[sector_num].height[1]);
  colorWipeScope(p[s[sector_num].j_hat[1]].Color(100,0,0), 0, s[sector_num].j_hat[1], s[sector_num].height[0], s[sector_num].height[1]);
}
void bright_allsector()
{
  for (int i = 0; i < 8; i++){
     bright_sector(i);
  }
}
*/
void bright_all()
{
  colorWipe(p0.Color(0,0,100), 0, 0);
}

// color wipe
void select(int n)
{
  selector = Adafruit_NeoPixel(NUMPIXELS, pin_num[n], NEO_GRB + NEO_KHZ800);
  selector.begin();
  selector.show();
}
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
  for (int i = start; i < finish; i++){
    selector.setPixelColor(i,c);
    selector.show();
    delay(wait);
  }
}

