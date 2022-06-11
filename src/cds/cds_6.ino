# define MAX_TILE = 6;

typedef struct tile{
  int tile_num;
  int pin_num;
  int cds;
  int parking; 
  int count;
  // p = 1 parking state
  // p = 0 empty
}ptile;





// init tile
ptile A[6];

for (int i; i< MAX_TILE; i++){
  A.tile_num = i;
  A.pin_num = i+10;
  A.cds = 0;
  A.parking = 0;
  A.count = 0;
}

int cds[6] = {A0, A1, A2, A3, A4, A5};
int led = 3;
void led_output(int cdsValue);
void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}
void loop()
{
  for (int i = 0; i< 6; i++){
    int cdsValue = analogRead(cds[i]);
    Serial.print("CDS_");
    Serial.print(i);
    Serial.print("=");
    Serial.println(cdsValue);
    led_output(cdsValue);
  }
}
void led_output(int cdsValue)
{
  if (cdsValue > 220){
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW);
  }
}
void isParking(int cdsValue)
{
  if ()
}
