  #include <Adafruit_NeoPixel.h>
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  
  const char* ssid = "hitschler did";
  const char* password = "3UP42ONIB7KZ";
  
  WiFiServer server(5151);
  
  #define PIN 2
  
  #define meters 5
  #define num_leds 30*meters
  
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_leds, PIN, NEO_GRB + NEO_KHZ800);
  
  
  WiFiClient theclient;
  void setup()
  {
    pinMode(PIN, OUTPUT);
    Serial.begin(9600);
    
    digitalWrite(PIN, HIGH);
    delay(3000);
    digitalWrite(PIN, LOW);
    
    WiFi.begin(ssid,password);
    
    Serial.println("started");
    
    int flick = 0;
    while(WiFi.status() != WL_CONNECTED)
    {
      flick =!flick;
      digitalWrite(PIN, flick);
      delay(500);
      Serial.println(".");
    }
    
    server.begin();
    theclient.stop();
    
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
   

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  for (int i=0;i<num_leds;i++)
  {
      strip.setPixelColor(i, 25, 0, 0);
      strip.show();
  }
 
}



void loop()
{

      
  if(!theclient.connected()||theclient.status()==CLOSED)
  {
    theclient.stop();
    theclient = server.available();
    if(!theclient.connected())
    {
      return;
    }
    theclient.print("[STRIP] Connection successful\n");
    //this part works
  }


  int receivedbytes = theclient.available();
  
  if(4>receivedbytes)
  {
    return;
  }

  bool unchanged = true;
  
  if(receivedbytes % 4 != 0)
  {
    unchanged=false;
    receivedbytes = receivedbytes - (receivedbytes % 4);
    return;
  }

  if(!receivedbytes)
  {
    return;
  }

  for(receivedbytes;receivedbytes!=0;receivedbytes--)
  {
    unsigned char num = theclient.read();
    unsigned char r = theclient.read();
    unsigned char g = theclient.read();
    unsigned char b = theclient.read();
  
    if(num&&r==0xFF&&g==0xFF&&b==0xFF)//eof
    {
      strip.show();
    }
  
    if(num>num_leds)
    {
      theclient.print("[STRIP] don't have that much leds!\n");
      return;
    }

    

    //char bananas[40] = {0};
    //sprintf(bananas, "ok: %x _ %x _ %x _ %x : %x\n", lednum, r, g, b, receivedbytes);
    //theclient.print(bananas);
    
    strip.setPixelColor(num, r, g, b);

  }

  if(unchanged)
  {
    theclient.flush();
  }
 

  

}
