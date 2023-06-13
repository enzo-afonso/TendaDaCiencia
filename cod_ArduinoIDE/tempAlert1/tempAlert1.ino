#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define ONE_WIRE_BUS 4
#define WIFI_SSID "Marcos"
#define WIFI_PASSWORD "mf37abcd"
#define FIREBASE_HOST "https://ledwifi-a54cf-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyC-PuRZzA8cw1xcetgVP-V2_UhDhbOZaLA"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
WiFiUDP ntpUDP;
String dateTime;
String tempoInicial;
String tempoFinal;

OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);
DHT dht(DHTPIN, DHTTYPE);
NTPClient timeClient(ntpUDP, "pool.ntp.org");

int buzzer = 5;
int a = 0;
float valorTemp = 0;
bool tempInadequada = false;

void setup() {
  Serial.begin(9600);

  dht.begin();
  pinMode(buzzer,OUTPUT);

  initWifi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  timeClient.begin();
  timeClient.setTimeOffset(-10800); 
}

void loop() {
  delay(1000);

  sensors.requestTemperatures();
  float t = sensors.getTempCByIndex(0);
  float tempEx = dht.readTemperature();

  
  Serial.print("Temperatura interna: ");
  Serial.print(t);
  Serial.println("°C ");
  Serial.print("Temperatura externa: ");
  Serial.print(tempEx);
  Serial.println("°C ");
  

  Firebase.setFloat(fbdo, "/temperatura/valor", t);
  Firebase.setFloat(fbdo, "/tempExterna/valor", tempEx);


  if (t < 2 || t > 8) {
    
    if (a == 0) {
      tempoInicial = timeStamp(tempoInicial);
      tempInadequada = true;
    }
    tempoFinal = timeStamp(tempoFinal);

    // alarme(buzzer);
 
    valorTemp = valorTemp + t;
    a = a+1;

  }
  else if (tempInadequada == true) {
    float tempMedia = valorTemp/a;
    String dados = String("As vacinas ficaram do dia " + String(tempoInicial) + " até o dia " + String(tempoFinal) + " a uma temperatura média de " + String(tempMedia) + "°C! Tenha cuidado!");
    sendStringDataToFirebase("/tempInadequada", dados);
    tempInadequada = false;
    a = 0;
    valorTemp = 0;
    noTone(buzzer);
  }
  else {
    noTone(buzzer);
  }
}

void initWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED){
    Serial.println("Conexão estabelecida!");
}
}

void sendStringDataToFirebase(String path, String data ){
  Firebase.RTDB.pushString(&fbdo, path, data); 
}

String timeStamp(String dateTime) {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);  
  dateTime = String(ptm->tm_mday) + "-" + String(ptm->tm_mon+1) +  "-" + String( ptm->tm_year+1900);
  dateTime += ", no horário de " + timeClient.getFormattedTime();
  return dateTime;
}

String getCurrentTime() {
  time_t now = time(nullptr);
  return String(hour(now)) + ":" + String(minute(now)) + ":" + String(second(now));
}

void alarme(int buzzer) {
  int i = 0;
  while (i < 10){
    tone(buzzer, 1350); 
    delay(150);
    tone(buzzer, 1050); 
    delay(150);
    i = i +1;
  }
}