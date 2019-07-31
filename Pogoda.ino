// Version v1.02 Works with the Wemos D1 board R2 http://bit.ly/WEMOS_D1

#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>

#define D0 3 // GPIO3 maps to Ardiuno D0
#define D1 1 // GPIO1 maps to Ardiuno D1
#define D2 16 // GPIO16 maps to Ardiuno D2
#define D3 5 // GPIO5 maps to Ardiuno D3
#define D4 4 // GPIO4 maps to Ardiuno D4
#define D5 0 // GPIO14 maps to Ardiuno D5
#define D6 2 // GPIO12 maps to Ardiuno D6
#define D7 14 // GPIO13 maps to Ardiuno D7
#define D8 12 // GPIO0 maps to Ardiuno D8
#define D9 13 // GPIO2 maps to Ardiuno D9
#define D10 15 // GPIO15 maps to Ardiuno D10

const char* ssid     = "iPhone";      // SSID of local network
const char* password = "tomaszek";   // Password on network
String APIKEY = "7e6eaf64fbc0d7f24fbfae7b78ef427e";
String CityID = "756135"; //Sparta, Greece
//String CityID = "264371"; //Athens, Greece


WiFiClient client;
char servername[]="api.openweathermap.org";  // remote server we will connect to
String result;

int  counter = 60;

String weatherDescription ="";
String weatherLocation = "";
String Country;
float Temperature;
float Humidity;
float Pressure;

LiquidCrystal lcd(D8,D9,D4,D5,D6,D7); 

void setup() {
  Serial.begin(115200);
  int cursorPosition=0;
  //lcd.begin(16, 2);
  //Serial.println("   Connecting");  
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(cursorPosition,2); 
    Serial.println(".");
    cursorPosition++;
  }
  //lcd.clear();
  //Serial.println("   Connected!");
  Serial.println("Connected");
  delay(1000);

}
void loop() {
    if(counter == 60) //Get new data every 10 minutes
    {
      counter = 0;
      displayGettingData();
      delay(1000);
      getWeatherData();
    }else
    {
      counter++;
      displayWeather(weatherLocation,weatherDescription);
      delay(5000);
      displayConditions(Temperature,Humidity,Pressure);
      delay(5000);
    }
}

void getWeatherData() //client function to send/receive GET request data.
{
  if (client.connect(servername, 80)) {  //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?id="+CityID+"&units=metric&APPID="+APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  } 
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
      result = result+c;
    }

  client.stop(); //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  Serial.println(result);

char jsonArray [result.length()+1];
result.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[result.length() + 1] = '\0';

StaticJsonDocument<1024> json_buf;
deserializeJson(json_buf, jsonArray);
//
//if (!root.success())
//{
//  Serial.printlnln("parseObject() failed");
//}

String location = json_buf["name"];
String country = json_buf["sys"]["country"];
float temperature = json_buf["main"]["temp"];
float humidity = json_buf["main"]["humidity"];
String weather = json_buf["weather"]["main"];
String description = json_buf["weather"]["description"];
float pressure = json_buf["main"]["pressure"];

weatherDescription = description;
weatherLocation = location;
Country = country;
Temperature = temperature;
Humidity = humidity;
Pressure = pressure;

}

void displayWeather(String location,String description)
{
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.println(location);
  Serial.println(", ");
  Serial.println(Country);
  lcd.setCursor(0,1);
  Serial.println(description);
}

void displayConditions(float Temperature,float Humidity, float Pressure)
{
  lcd.clear();
  Serial.println("T:"); 
 Serial.println(Temperature,1);
 Serial.println((char)223);
 Serial.println("C ");
 
 //Printing Humidity
 Serial.println(" H:");
 Serial.println(Humidity,0);
 Serial.println(" %");
 
 //Printing Pressure
 lcd.setCursor(0,1);
 Serial.println("P: ");
 Serial.println(Pressure,1);
 Serial.println(" hPa");

}

void displayGettingData()
{
  lcd.clear();
  Serial.println("Getting data");
}

