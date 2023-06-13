#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);

#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "ACASSIA 2Ghz"
#define WIFI_PASSWORD "cadeocachorro"

#define FIREBASE_HOST "https://ledwifi-a54cf-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyC-PuRZzA8cw1xcetgVP-V2_UhDhbOZaLA"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <TimeLib.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String dateTime;

int buzzer = 5; // Pino D1

DHT dht(DHTPIN, DHTTYPE);

int a = 0;
float valorTemp = 0;
bool tempInadequada = false;
String tempoInicial;
String tempoFinal;


void setup() {
  Serial.begin(9600);

  dht.begin();
  pinMode(buzzer,OUTPUT);

  initWifi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  timeClient.begin();
  timeClient.setTimeOffset(-10800); // -10800 => GMT -3
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

  //sendFloatDataToFirebase("/tempInadequada/valor", t);
  //sendStringDataToFirebase("/tempInadequada/horario", dateTime);


  int i = 0;


  if (t < 2 || t > 8) {
    
    if (a == 0) {
      //tempoInicial = getCurrentTime();
      tempoInicial = timeStamp(tempoInicial);
      tempInadequada = true;
    }
    tempoFinal = timeStamp(tempoFinal);
    while (i < 10){
      tone(buzzer, 1350); 
      delay(150);
      tone(buzzer, 1050); 
      delay(150);
      i = i +1;
    }
    valorTemp = valorTemp + t;
    a = a+1;

  }else if (tempInadequada == true){
    noTone(buzzer);
    float tempMedia = valorTemp/a;
    String dados = String("As vacinas ficaram do dia " + String(tempoInicial) + " até o dia " + String(tempoFinal) + " a uma temperatura média de " + String(tempMedia) + "°C! Tenha cuidado!");
    //Firebase.pushString(fbdo, "/tempInadequada", dados);
    sendStringDataToFirebase("/tempInadequada", dados);
    tempInadequada = false;
    a = 0;
  } else {
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
  time_t now = time(nullptr); // obter a hora atual como um objeto "time_t"
  return String(hour(now)) + ":" + String(minute(now)) + ":" + String(second(now)); // retornar a hora atual em formato de string
}