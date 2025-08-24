int sensor=0;
int newMap=0;

const int GREEN_LED_PIN = D5;  
const int RED_LED_PIN = D7;    
const int SENSOR_PIN = A0;   

void setup() {

  Serial.begin(9600);


  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
}

void loop() {

  sensor = analogRead(SENSOR_PIN);


  newMap = map(sensor, 13, 1024, 0, 100);


  Serial.print("Mapped Value: ");
  Serial.println(newMap);


  if (newMap % 2 == 0) {

    digitalWrite(RED_LED_PIN, LOW);   
    
    digitalWrite(GREEN_LED_PIN, HIGH); 
    delay(200);                        
    digitalWrite(GREEN_LED_PIN, LOW);  
    delay(200);                       

  } else {

    digitalWrite(GREEN_LED_PIN, LOW); 

    digitalWrite(RED_LED_PIN, HIGH);  
    delay(200);                       
    digitalWrite(RED_LED_PIN, LOW);   
    delay(200);                       
  }


  delay(600); 
}