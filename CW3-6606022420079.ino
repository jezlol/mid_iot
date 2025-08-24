#include <DHT.h>
#define DHT_PIN D4
#define RED_LED D5
#define YELLOW_LED D6
#define GREEN_LED D7
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

float currentTemp = 0.0;
float previousTemp = 0.0;
bool isFirstReading = true;

unsigned long previousMillis = 0;
const long blinkInterval = 300;
bool ledState = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  
  dht.begin();
  
  Serial.println("DHT22 Temperature Monitor Started");
  delay(2000);
}

void loop() {
  float temp = dht.readTemperature();
  
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("°C");
  
  currentTemp = temp;
  
  if (!isFirstReading) {
    if (currentTemp > previousTemp) {
      Serial.println("Temperature Rising!");
      blinkSequence(1);
    } else if (currentTemp < previousTemp) {
      Serial.println("Temperature Falling!");
      blinkSequence(2);
    } else {
      Serial.println("Temperature Stable");
      allLEDsOff();
    }
  } else {
    Serial.println("First reading - establishing baseline");
    isFirstReading = false;
  }
  previousTemp = currentTemp;
  
  delay(3000);
}

void blinkSequence(int mode) {
  if (mode == 1) {
    for (int i = 0; i < 1; i++) {
      digitalWrite(RED_LED, HIGH);
      delay(300);
      digitalWrite(RED_LED, LOW);
      delay(200);
      digitalWrite(YELLOW_LED, HIGH);
      delay(300);
      digitalWrite(YELLOW_LED, LOW);
      delay(200);
      digitalWrite(GREEN_LED, HIGH);
      delay(300);
      digitalWrite(GREEN_LED, LOW);
      delay(200);
    }
  } else if (mode == 2) {
    for (int i = 0; i < 1; i++) {
      digitalWrite(GREEN_LED, HIGH);
      delay(300);
      digitalWrite(GREEN_LED, LOW);
      delay(200);
      digitalWrite(YELLOW_LED, HIGH);
      delay(300);
      digitalWrite(YELLOW_LED, LOW);
      delay(200);
      digitalWrite(RED_LED, HIGH);
      delay(300);
      digitalWrite(RED_LED, LOW);
      delay(200);
    }
  }
}

void allLEDsOff() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}