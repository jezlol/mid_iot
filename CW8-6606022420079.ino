#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID           "TMPL6NwJLxHkd"
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "Tf1gGzw3U36JPWOmLvksACV2J7Ivz15j"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
char ssid[] = "Jez 💀💀💀";
char pass[] = "jezlol777";
int count = 0;
#define DHTPIN D5         
#define DHTTYPE DHT22     
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  

  count++;


  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V0, count); 


  Serial.print("Count: ");
  Serial.println(count);
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{

  Blynk.run();
  timer.run();
}