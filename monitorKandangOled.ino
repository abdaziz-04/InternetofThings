#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 

//BUZZER
#define buzzerPin 3

//LED
#define ledBiru 7
#define ledKuning 6
#define ledMerah 5

//DHT
#define DHTPIN 2
#define DHTTYPE DHT22

//OLED
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); 
DHT dht(DHTPIN, DHTTYPE);

String displayString;

void setup() {
  Serial.begin(9600);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 failed"));
    while (true);

    //OUT
    pinMode(ledBiru, OUTPUT);
    pinMode(ledKuning, OUTPUT);
    pinMode(ledMerah, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
  }
  
  oled.clearDisplay(); 

  oled.setTextSize(2);      
  oled.setTextColor(WHITE); 
  oled.setCursor(0, 10);    

  dht.begin();             

  displayString.reserve(10); 
}

void loop() {
  float tempC = dht.readTemperature(); 

  //CHECK
  if (isnan(tempC)) {
    displayString = "Failed";
  } else {
    if (tempC < 29 ) {
      displayString = "SUHU RENDAH";
      digitalWrite(ledBiru, HIGH);
      digitalWrite(ledMerah, LOW);
      digitalWrite(ledKuning, LOW);
      noTone(buzzerPin);
    } else if (tempC > 29 && tempC < 35 ) {
      displayString = "SUHU CUKUP";
      digitalWrite(ledKuning, HIGH);
      digitalWrite(ledMerah, LOW);
      digitalWrite(ledBiru, LOW);
      noTone(buzzerPin);
    } else if (tempC > 35) {
      displayString = "SUHU PANAS";
      digitalWrite(ledMerah, HIGH);
      digitalWrite(ledBiru, LOW);
      digitalWrite(ledKuning, LOW);
      tone(buzzerPin, 1000);
    }
  }

  oledDisplayCenter(displayString); 
}

void oledDisplayCenter(String text) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  oled.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

  oled.clearDisplay(); 
  oled.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
  oled.println(text); 
  oled.display();
}