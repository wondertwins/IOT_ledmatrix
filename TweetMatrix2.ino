#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 10

//ESP stuff
const char* ssid     = "CaseGuest";
const char* password = "";

const char* host = "api.thingspeak.com";

String tweet = "";

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(10, 10, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

void setup() {
  Serial.begin(9600);
  delay(1000);

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
    
  
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(100);
  matrix.setTextColor(colors[1]);
}

int x    = matrix.width();
int pass = 0;

void loop() 
{
  delay(1000);
  getTweet();
  //for(int z=0;z<3;z++)
  //{
  int pixelsInText = (tweet.length() * 7)+8;
  while(x+17 > (matrix.width() - pixelsInText))
  {
    matrix.fillScreen(colors[0]);
    matrix.setCursor(--x, 0);
    matrix.print(tweet);
    matrix.show();
    //Serial.println("Displaying " + tweet);
    delay(50);
  }
  x = matrix.width();
  delay(1000);
  //}
}

void getTweet()
{
  delay(1000);
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
  String url = "/channels/225462/fields/1/last.txt";
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
    
    if(x == 19) //line number where the text is in the html
    {
      Serial.println(line.substring(22)); //cut text after hashtags
      tweet = line.substring(22);
    }
    
    
    //String line = client.readStringUntil('\r');
    //Serial.println(line + "LineNumber: " + x);
    x++;
  }
  
  Serial.println();
  Serial.println("closing connection");
  
}
