#include<Arduino.h>
#include<TFT_eSPI.h>
#include<SPI.h> // used to connect to onboard display
#include<WiFi.h> // used to connect to internet
#include<WebServer.h> // used to set ESP board as a server
#include "../utility/content.cpp" // access HTML for webpage
#include <dht11.h>
// internet credentials
#define WIFI_NETWORK "FG-Network"
#define WIFI_PASSWORD "Felipe0331"
#define WIFI_TIMEOUT 20000  // if its been more than 20 seconds stop trying to connect

WebServer server(80); // Webserver object
TFT_eSPI tft = TFT_eSPI(); // onboard display object
dht11 dht;

// function prototypes
void sendHomePage();
void connectToWifi();
void handleLight();
void handleTemperature();
void handleHumidity();
int getLight();
int getTemperature();
int getHumidity();
void startUpDisplay();

#define LIGHTPIN 32
#define DHTPIN 33

//============================================
void setup() {
  Serial.begin(9600);
  connectToWifi();

  pinMode(LIGHTPIN,INPUT); // set pin to input mode - allows us to analogRead(PIN)

  disableCore0WDT(); // disable watchdog timer - this timer would normally shutdown the server if there is no activity

  // HTTP requests
  server.on("/", sendHomePage); // on connection this function is called = '/' is the root directory in HTML
  server.on("/readLight", handleLight); // Directly displays the value of light sensor - or any data being read
  server.on("/readTemperature", handleTemperature);
  server.on("/readHumidity", handleHumidity);
  server.begin(); // start the server before starting the loop

  startUpDisplay(); 
}

void loop() {
  server.handleClient(); // handle connections to the server- need to be called continuously
}

void startUpDisplay(){
  tft.init();
  tft.fillScreen(TFT_WHITE); // fill background with white
  tft.setTextColor(TFT_BLACK);
  tft.setRotation(1);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.println("connected to");
  tft.println( WiFi.localIP());
  delay(5000);

  tft.fillScreen(TFT_WHITE);
  tft.println("Program Starting");
  delay(3000);
  digitalWrite(4,LOW); // turn off ttgo display

}

/*
This function is called by server.on() - sends 200 status code, the type of content to expect
and the contents of HOME_PAGE are displayed (contents store on another file)
*/
void sendHomePage(){
  Serial.println("Sending home page..."); // prints to monitor - some one connected 
  server.send(200,"text/html", HOME_PAGE); // response code 200, what to expect, and function/parameter to return
}

/*
This funtion reads from the light sensor, converts the value to a string, and updates the value with ajax
*/
void handleLight() {
 String lightValue = String(getLight()); // cast value to string - must be cast to string to be rendered in HTML
 
 server.send(200, "text/plane", lightValue); // send the string value as parameter
}

void handleTemperature() {
 String temperature = String(getTemperature()); // cast value to string - must be cast to string to be rendered in HTML
 server.send(200, "text/plane", temperature); // send the string value as parameter
}

void handleHumidity(){
  String humidity = String(getHumidity());
  server.send(200,"text/plane", humidity);
}

int getLight(){
  int light= 0 ;
  for(int i = 0; i < 20; i++){
    light += analogRead(LIGHTPIN);
  }
  return light/20;
}

int getTemperature(){
  int check = dht.read(DHTPIN);
  return (dht.temperature * (9/5)) + 32 ;
}

int getHumidity(){
  int check = dht.read(DHTPIN);
  return dht.humidity;
}

void connectToWifi(){
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA); // station mode - connects to wifi
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD); // pass credentials to access internet
  unsigned long startConnectionTime = millis(); // store time since starting program - will be low value

  // while we are not connected and havent passed the timeout threshold - keep trying to connect
  while(WiFi.status() != WL_CONNECTED && (millis() - startConnectionTime) < WIFI_TIMEOUT){
      Serial.println('.');
      delay(100); // short wait before checking status again
  }
  
  if(WiFi.status() != WL_CONNECTED){                                  // if we failed to connect stop the program from continuing.
      Serial.println("Failed to connect to " + String(WIFI_NETWORK)); // and inform the user
      while(1){} // stop the code from progressing.
  }else{
      Serial.print("Connected to ");
      Serial.println( WiFi.localIP());
  }
}