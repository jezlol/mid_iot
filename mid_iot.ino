/********* Blynk *********/
#define BLYNK_TEMPLATE_ID   "TMPL6A8WemwbM"
#define BLYNK_TEMPLATE_NAME "exam"
#define BLYNK_AUTH_TOKEN    "uIlHoR2cxN-lzJmpCmO-tVmqmt8i68B8"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

/* ==== Config ==== */
// ถ้าไฟแดงของคุณต่อแบบ active-LOW ให้เปลี่ยนเป็น true
const bool RED_ACTIVE_LOW = false;    // <<<<< ลองแบบนี้ก่อน

const float TEMP_HIGH_C   = 30.0;     // เกณฑ์อุณหภูมิ

/* ---- Pins ---- */
const uint8_t PIN_RED    = D4;   // เปลี่ยนขาได้ถ้าจำเป็น
const uint8_t PIN_YELLOW = D5;
const uint8_t PIN_GREEN  = D6;
const uint8_t PIN_SW     = D7;

/* DHT22 at D1 (GPIO5) */
const uint8_t PIN_DHT = D1;
#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);

/* Wi-Fi */
char ssid[] = "Jez777";
char pass[] = "jezlol777";

/* State */
BlynkTimer timer;
bool  yellowLatched = false;
float lastTemp = NAN;

/* ===== Helpers ===== */
inline void redWrite(bool on){
  if (RED_ACTIVE_LOW) digitalWrite(PIN_RED, on ? LOW : HIGH);
  else                digitalWrite(PIN_RED, on ? HIGH: LOW );
}
inline void ledWrite(uint8_t pin, bool on){
  if (pin == PIN_RED) { redWrite(on); return; }
  digitalWrite(pin, on ? HIGH : LOW);   // GREEN/YELLOW เป็น active-HIGH
}
inline void setAll(bool r, bool g, bool y){
  redWrite(r);
  digitalWrite(PIN_GREEN, g ? HIGH : LOW);
  digitalWrite(PIN_YELLOW, y ? HIGH : LOW);
}
inline void setRGbyTemp(float t){
  bool hot = (t >= TEMP_HIGH_C);
  redWrite(hot);                       // แดง = ร้อน
  digitalWrite(PIN_GREEN, hot ? LOW : HIGH);  // เขียว = ไม่ร้อน
}

/* ===== Tasks ===== */
void readDHTTask(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) { Serial.println("[DHT] read failed"); return; }

  lastTemp = t;
  Blynk.virtualWrite(V1, (int)round(h));  // Humidity (Integer)
  Blynk.virtualWrite(V2, t);              // Temp (Double)
  Serial.printf("[DHT] H=%d %%  T=%.1f C\n", (int)round(h), t);

  // อัปเดต R/G ทุกครั้งให้สวนทางกันเสมอ
  setRGbyTemp(t);
}

void pollSwitchTask(){
  static uint8_t last = HIGH;
  static uint32_t tdeb = 0;
  uint8_t now = digitalRead(PIN_SW);          // LOW = กด
  if (now != last && (millis() - tdeb) > 40) {
    last = now; tdeb = millis();
    if (now == LOW) {
      yellowLatched = !yellowLatched;
      ledWrite(PIN_YELLOW, yellowLatched);
      Blynk.virtualWrite(V3, yellowLatched ? 1 : 0);
      Serial.printf("[SW] Yellow -> %s\n", yellowLatched ? "ON" : "OFF");
    }
  }
}

/* ===== Blynk Handlers ===== */
BLYNK_WRITE(V3){                      // Switch ในแอปคุม Yellow
  int v = param.asInt();
  yellowLatched = (v == 1);
  ledWrite(PIN_YELLOW, yellowLatched);
  Serial.printf("[APP] V3=%d, Yellow %s\n", v, yellowLatched ? "ON" : "OFF");
}

BLYNK_CONNECTED(){
  Serial.println("[Blynk] Connected");
  Blynk.syncVirtual(V3);
}

/* ===== Setup/Loop ===== */
void setup(){
  Serial.begin(115200);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_SW, INPUT_PULLUP);

  // ปิดไฟทุกดวงแบบ “เคารพโพลาริตี้”
  setAll(false, false, false);

  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, readDHTTask);
  timer.setInterval(50L,   pollSwitchTask);
}

void loop(){
  Blynk.run();
  timer.run();
}
