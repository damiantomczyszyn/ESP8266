#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
//#include <Arduino_JSON.h>
#include <ArduinoJson.h>
const char* ssid     = "s";     // your network SSID (name of wifi network)
const char* password = ""; // your network password

const char*  server = "weather.visualcrossing.com";  // Server URL
WiFiClientSecure client;

//const size_t capacity = JSON_OBJECT_SIZE(1000) ;
//StaticJsonDocument<capacity> doc;
const size_t capacity = JSON_OBJECT_SIZE(1000) + 300;
DynamicJsonDocument doc(capacity);

String jsonResponse = "";

unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;

void printDayInfo(JsonObject day, int dayNumber);//deklaracja

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(100);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
}

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("\nStarting connection to server...");
  client.setInsecure();//skip verification
  if (!client.connect(server, 443))
    Serial.println("Connection failed!");
  else {
    Serial.println("Connected to server!");
    // Make a HTTP request:
    client.println("GET https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/Lublin/2024-01-10/2024-01-14?unitGroup=metric&elements=datetime%2Ctempmax%2Ctempmin%2Ctemp%2Cdew%2Chumidity%2Cprecip%2Cprecipprob%2Cpreciptype%2Cwindspeed%2Cwindspeedmax%2Cwindspeedmean%2Cwindspeedmin%2Cpressure%2Ccloudcover%2Cuvindex%2Csunrise%2Csunset&include=fcst%2Cremote%2Cobs%2Cdays&key= HTTP/1.0");
    client.println("Host: weather.visualcrossing.com");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      
      Serial.println(line); // HTTP/1.1 200
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    Serial.println("teraz znaki");
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
      if(c=='{')
      {
        jsonResponse = "{";
        while (client.available())
        {
                char c = client.read();
                Serial.write(c);
          jsonResponse+=c;
        }
      }
    }
    Serial.println();
    client.stop();



  //to json
    deserializeJson(doc, jsonResponse);
      //Serial.println("Estimated memory usage: " + String(measureJson(doc)) + " bytes");
    if (doc.isNull()) {
        Serial.println("Parsing input failed!");
        return;
      }
    
    Serial.println("JSON object = ");
    serializeJsonPretty(doc, Serial);
    Serial.println("Estimated memory usage: " + String(measureJson(doc)) + " bytes");
    

  }

  // Dostęp do poszczególnych elementów
  double latitude = doc["latitude"];
  double longitude = doc["longitude"];
  const char* resolvedAddress = doc["resolvedAddress"];
  const char* address = doc["address"];
  const char* timezone = doc["timezone"];
  int tzoffset = doc["tzoffset"];


  Serial.println("Latitude: " + String(latitude));
  Serial.println("Longitude: " + String(longitude));
  Serial.println("Resolved Address: " + String(resolvedAddress));
  Serial.println("Address: " + String(address));
  Serial.println("Timezone: " + String(timezone));
  Serial.println("TZ Offset: " + String(tzoffset));

for (int x = 0; x < 5; x++) {
    // Dostęp do elementów w tablicy "days"
    JsonObject days = doc["days"][x];
    printDayInfo(days, x + 1);
  }



}



void loop() {
 
}


void printDayInfo(JsonObject day, int dayNumber) {
  const char* datetime = day["datetime"];
  double tempmax = day["tempmax"];
  double tempmin = day["tempmin"];
  double temp = day["temp"];
  double dew = day["dew"];
  double humidity = day["humidity"];
  double precip = day["precip"];
  double precipprob = day["precipprob"];
  const char* preciptype = day["preciptype"]; // Ostrzeżenie: To pole może być null
  double windspeed = day["windspeed"];
  double pressure = day["pressure"];
  double cloudcover = day["cloudcover"];
  double uvindex = day["uvindex"];
  double windspeedmax = day["windspeedmax"];
  double windspeedmean = day["windspeedmean"];
  double windspeedmin = day["windspeedmin"];
  const char* sunrise = day["sunrise"];
  const char* sunset = day["sunset"];

  Serial.println("Day " + String(dayNumber) + " Info:");
  Serial.println("Date Time: " + String(datetime));
  Serial.println("Temp Max: " + String(tempmax));
  Serial.println("Temp Min: " + String(tempmin));
  Serial.println("Temp: " + String(temp));
  Serial.println("Dew: " + String(dew));
  Serial.println("Humidity: " + String(humidity));
  Serial.println("Precip: " + String(precip));
  Serial.println("Precip Prob: " + String(precipprob));
  Serial.println("Precip Type: " + String(preciptype)); // Może być null
  Serial.println("Wind Speed: " + String(windspeed));
  Serial.println("Pressure: " + String(pressure));
  Serial.println("Cloud Cover: " + String(cloudcover));
  Serial.println("UV Index: " + String(uvindex));
  Serial.println("Wind Speed Max: " + String(windspeedmax));
  Serial.println("Wind Speed Mean: " + String(windspeedmean));
  Serial.println("Wind Speed Min: " + String(windspeedmin));
  Serial.println("Sunrise: " + String(sunrise));
  Serial.println("Sunset: " + String(sunset));
  Serial.println();
}
