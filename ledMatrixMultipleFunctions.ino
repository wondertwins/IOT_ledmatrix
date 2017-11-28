//best one yet no 5v vin. need to do a 3.3v regualtor or something voltage deiveder
#include <ESP8266WiFi.h>

const char* ssid     = "CaseGuest";
const char* password = "";

const char* host = "api.thingspeak.com";

#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    100
#define BRIGHTNESS  100
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

int width = 10;
int height = 10;

int spotsArray[6][15] = {{99,  98,  97, //this array defines the cells or spots of where the numbers can be masked into
                      80,  81,  82,
                      79,  78,  77,
                      60,  61,  62,
                      59,  58,  57},
                      {96,  95,  94,
                      83,  84,  85,
                      76,  75,  74,
                      63,  64,  65,
                      56,  55,  54},
                      {93,  92,  91,
                      86,  87,  88,
                      73,  72,  71,
                      66,  67,  68,
                      53,  52,  51},
                      {40,  41,  42,
                      39,  38,  37,
                      20,  21,  22,
                      19,  18,  17,
                       0, 1, 2},
                       {43,  44,  45,
                      36, 35,  34,
                      23,  24,  25,
                      16,  15,  14,
                      3, 4, 5},
                      {46, 47,  48,
                      33,  32,  31,
                      26,  27,  28,
                      13,  12,  11,
                      6, 7, 8}};

//Color Palette Variabels
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() 
{
  
    Serial.begin(115200);
    delay(1000); //1 sec power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    //Color Palette Stuff
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    //wifi connection stuff
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}


  //LightUpNumbers("02:42");
  //Serial.println(CRGB::Red);
  //delay(500);

void LightUpNumbers(String input) //takes a string with no spaces but can take a colon (up to six char) and dispalys in alternating colors
{
  int ledMask[11][15] = {
                   {1,1,1,
                   1,0,1,
                   1,0,1,
                   1,0,1,
                   1,1,1},
                   {0,1,0, //defines which leds should be ON to produce the correct number
                   0,1,0,
                   0,1,0,
                   0,1,0,
                   0,1,0},
                   {1,1,1,
                   0,0,1,
                   1,1,1,
                   1,0,0,
                   1,1,1},
                   {1,1,1,
                   0,0,1,
                   1,1,1,
                   0,0,1,
                   1,1,1},
                   {1,0,1,
                   1,0,1,
                   1,1,1,
                   0,0,1,
                   0,0,1},
                   {1,1,1,
                   1,0,0,
                   1,1,1,
                   0,0,1,
                   1,1,1,},
                   {1,0,0,
                   1,0,0,
                   1,1,1,
                   1,0,1,
                   1,1,1},
                   {1,1,1,
                   0,0,1,
                   0,0,1,
                   0,0,1,
                   0,0,1},
                   {1,1,1,
                   1,0,1,
                   1,1,1,
                   1,0,1,
                   1,1,1},
                   {1,1,1,
                   1,0,1,
                   1,1,1,
                   0,0,1,
                   0,0,1},
                   {0,0,0,
                   0,1,0,
                   0,0,0,
                   0,1,0,
                   0,0,0}};
  
  //first clear led
  for(int x=0;x<NUM_LEDS;x++)
  {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  
  char inputNumbers[input.length()];
  
  for(int x=0;x<input.length();x++)
    inputNumbers[x] = input.charAt(x);

  
  
  //Serial.println(inputNumbers[0]);
  //Serial.println("done");

  for(int x=0;x<input.length();x++)
  {
   
    
    if(inputNumbers[x] != ':')
    {
      int spotNum = inputNumbers[x] - '0';
      //Serial.println(spotNum - 1);
      for(int z=0;z<15;z++)
      {
        if(ledMask[spotNum][z] == 1)
        {
          if(x % 2 == 0) //if x is even make color blue else make red
            leds[spotsArray[x][z]] = CRGB::Blue; //blue
          else
            leds[spotsArray[x][z]] = CRGB::Red; //red
          
        }
      }
      
    }
    else //if it is not a number it is a colon
    {
      for(int z=0;z<15;z++)
      {
        if(ledMask[10][z] == 1) //ten in colon spot
        {
         if(x % 2 == 0) //if x is even make color blue else make red
            leds[spotsArray[x][z]] = CRGB::Blue; //blue
          else
            leds[spotsArray[x][z]] = CRGB::Red; //red
        }
      }
    }
  }
  FastLED.show();
  delay(1000);
}

int value = 0;
void displayTime()
{
  delay(30000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/apps/thinghttp/send_request?api_key=R11B1WX0DAK8QX3A";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  int x = 0;
  while(client.available())
  {

    String line = client.readStringUntil('\r');
    
    if(x == 18)
    {
      Serial.println(line);
      LightUpNumbers(line.substring(0,6));
    }
    
    
    //String line = client.readStringUntil('\r');
    //Serial.println(line + "LineNumber: " + x);
    x++;
  }
  
  Serial.println();
  Serial.println("closing connection");
}

void loop()
{
  displayTime();
  delay(1000);
//  ColorPaletteSetup();
  ColorPaletteloop();
}

