#include <Arduino.h>
#include <DHT.h>

// zadeklarowanie czujnika temperatury
DHT dht(26, DHT22);

//zadeklarowanie czujników przerwania i krańcówki
const int przerwanie = 4;
const int krancowka = 5;
const int zalaczenieZasilaniaNaGrzalke = 6;

//wartosci czujnikow
int przerwanieStan = 0;
int krancowkaStan = 0;

//zadana temperatura przez uzytkownika
float temperatura = 40.5;

void setup() {
  // put your setup code here, to run once:
  dht.begin();
  delay(2000);

  pinMode(przerwanie, INPUT);
  pinMode(krancowka, INPUT);
  pinMode(zalaczenieZasilaniaNaGrzalke, OUTPUT);

  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = dht.readTemperature();
  przerwanieStan = digitalRead(przerwanie);
  krancowkaStan = digitalRead(krancowka);
  //sprawdzamy czy temperatura jest w zakresie gdzie musimy grzac
  //sprawdzamy czy czujniki sa zamkniete
  if(przerwanieStan == HIGH && krancowkaStan == HIGH){
  
     if(temp > temperatura+2){

      digitalWrite(zalaczenieZasilaniaNaGrzalke, LOW);
    }
    else if(temp <= temperatura-2){

      digitalWrite(zalaczenieZasilaniaNaGrzalke, HIGH);

    }
  }

}
