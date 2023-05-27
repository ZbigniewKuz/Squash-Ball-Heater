#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <string>

using namespace std;

const char* ssid = "FunBox2-0755";
const char* password = "6F31A7CDFFA3A25DED94FDC69F";

//nazwy serewerów do metod
const char* serverDlaGet = "http://zbigniewk.pythonanywhere.com/machine/1";
const char* serverDlaPost = "http://zbigniewk.pythonanywhere.com/action/add";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (4000)
unsigned long timerDelay = 4000;

// zadeklarowanie czujnika temperatury
DHT dht(4, DHT22);

//zadeklarowanie czujnikĂłw przerwania i kraĹ„cĂłwki
const int przerwanie = 5;

//const int krancowka = 5;
const int zalaczenieZasilaniaNaGrzalke = 2;

//wartosci czujnikow
//int przerwanieStan = 0;

//int krancowkaStan = 0;

//odczytana temperatura z czytnika
float temperaturaOdczytana = 0;

//zmienne do odczytu z get
String tempGet;
float tempGetArr[4];
bool status = false;



void setup() {
  
  dht.begin();
  delay(2000);
  pinMode(przerwanie, INPUT);

  //pinMode(krancowka, INPUT);
  pinMode(zalaczenieZasilaniaNaGrzalke, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Łączenie");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Połączono z siecią Wifi o adresie: ");
  Serial.println(WiFi.localIP());
 
}

void loop() {
  //odczyt danych z czujnikĂłw
  temperaturaOdczytana = dht.readTemperature();
  //co 4 sek get request
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      postRequest(serverDlaPost, temperaturaOdczytana);        
      tempGet = getRequest(serverDlaGet);
      Serial.println(tempGet);
      JSONVar myObject = JSON.parse(tempGet);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
    
      // myObject.keys() can be used to get an array of all the keys in the object
      JSONVar keys = myObject.keys();
    
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        Serial.print(keys[i]);
        Serial.print(" = ");
        Serial.println(value);
        tempGetArr[i] = double(value);
        if(i == 3){
          status = bool(value);
        }
      }
      Serial.print("Status = ");
      Serial.println(status);
      Serial.print("1 = ");
      Serial.println(tempGetArr[0]);
      Serial.print("2 = ");
      Serial.println(tempGetArr[1]);
      Serial.print("3 = ");
      Serial.println(tempGetArr[2]);
      Serial.print("4 = ");
      Serial.println(tempGetArr[3]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
 // czujnikPrzerwaniaStan = digitalRead(przerwanie);
  //krancowkaStan = digitalRead(krancowka);
 
  delay(1000);
 
  //sprawdzamy czy temperatura jest w zakresie gdzie musimy grzac
  //sprawdzamy czy czujniki sa zamkniete

  if((temperaturaOdczytana > tempGetArr[2]+2)||(status == 0)){

    digitalWrite(zalaczenieZasilaniaNaGrzalke, LOW);
  }
  else if((temperaturaOdczytana <= tempGetArr[2]-2)&&(status == 1)){

    digitalWrite(zalaczenieZasilaniaNaGrzalke, HIGH);

  }
 
}

String getRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void postRequest(const char* serverName, float data){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/json");
      // Data to send with HTTP POST
     
      //String temperatura = to_string(data);
      char _post[60];
      sprintf(_post, "{\"machine_id\":\"1\",\"action_temperature\":\"%f\"}", data);
      Serial.println(_post);

     // String doRequesta ="{\"machine_id\":\"1\",\"action_temperature\":\""+ data +"\"}";
      int httpResponseCode = http.POST(_post);      
      // Send HTTP POST request
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
}




