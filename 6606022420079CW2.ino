const int switchPin1 = D0;
const int switchPin2 = D4;
const int redLED = D5;
const int yellowLED = D6;
const int greenLED = D7;

int count1 = 0;
int count2 = 0;
int countBoth = 0;

int button1Old = HIGH;
int button2Old = HIGH;

void setup() {
  Serial.begin(9600);
  
  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
}

void loop() {
  int button1 = digitalRead(switchPin1);
  int button2 = digitalRead(switchPin2);
  

  if (button1 == LOW && button2 == LOW) {
    if (button1Old == HIGH || button2Old == HIGH) {
      countBoth = countBoth + 1;
      Serial.print("Both buttons pressed: ");
      Serial.println(countBoth);
      
      if (countBoth == 7) {
        Serial.println("Yellow LED blinks 7 times");
        for (int i = 0; i < 7; i++) {
          digitalWrite(yellowLED, HIGH);
          delay(300);
          digitalWrite(yellowLED, LOW);
          delay(300);
        }
        countBoth = 0;
        count1 = 0;
        count2 = 0;
      }
    }
  }

  else if (button1 == LOW && button2 == HIGH) {
    if (button1Old == HIGH) {
      count1 = count1 + 1;
      Serial.print("Button 1 pressed: ");
      Serial.println(count1);
      
      if (count1 == 5) {
        Serial.println("Red LED blinks 5 times");
        for (int i = 0; i < 5; i++) {
          digitalWrite(redLED, HIGH);
          delay(300);
          digitalWrite(redLED, LOW);
          delay(300);
        }
        count1 = 0;
      }
    }
  }

  else if (button1 == HIGH && button2 == LOW) {
    if (button2Old == HIGH) {
      count2 = count2 + 1;
      Serial.print("Button 2 pressed: ");
      Serial.println(count2);
      
      if (count2 == 3) {
        Serial.println("Green LED blinks 3 times");
        for (int i = 0; i < 3; i++) {
          digitalWrite(greenLED, HIGH);
          delay(300);
          digitalWrite(greenLED, LOW);
          delay(300);
        }
        count2 = 0;
      }
    }
  }
  

  button1Old = button1;
  button2Old = button2;
  
  delay(50);
}