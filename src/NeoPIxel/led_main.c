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
