#include <Adafruit_NeoPixel.h>

#define PIN 13

#define meters 5
#define num_leds 30*meters

Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_leds, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  while(!Serial)
  {
    
  }

  for (int i=0;i<num_leds;i++)
  {
      strip.setPixelColor(i, 0xFF, 0, 0);
      strip.show();
  }

  Serial.flush();
  
}




void loop()
{
  
  int availablebytes = Serial.available();
  
  if(!availablebytes)
    return;


  
  if(availablebytes % 4 != 0)
  {
    if(4>availablebytes)
      return;

    while(availablebytes%4 != 0)
    {
      availablebytes--;
    }
    return;
  }


  
  for( availablebytes;availablebytes!=0;availablebytes--)
  {
    unsigned char lednum = Serial.read();

    if(lednum>num_leds)
    {
      continue;
    }
    
    unsigned char r = Serial.read();
    unsigned char g = Serial.read();
    unsigned char b = Serial.read();
    


    //char bananas[40] = {0};
    //sprintf(bananas, "ok: %x _ %x _ %x _ %x : %x\n", lednum, r, g, b, availablebytes);
    //Serial.println(bananas);
  
    strip.setPixelColor(lednum, r, g, b);


  }

  strip.show();
}
