
//#include <DHT.h>
//OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/Picopixel.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1)

//DHTSENSOR
#define DHTPIN 2     
#define DHTTYPE DHT22   
#define buzzerPin 3
DHT dht(DHTPIN, DHTTYPE);

//LED
#define ledBiru 4
#define ledKuning 5
#define ledMerah 6

//BUZZER
#define buzzerPin 3

void setup() {
  pinMode();
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
 
  float hic = dht.computeHeatIndex(t, h, false);

  /*Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));*/

    display.clearDisplay();
  display.setFont();
  display.setTextColor(1);

  display.setCursor(42, 2);
  display.print("Volume");
}