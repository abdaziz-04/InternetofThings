#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#define pinRelay 2

#include "ThingsBoard.h"
#include "DHTesp.h"

#define CURRENT_FIRMWARE_TITLE    "TEST"
#define CURRENT_FIRMWARE_VERSION  "1.0.0"

#define WIFI_SSID           "Wokwi-GUEST"
#define WIFI_PASSWORD       ""

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token


#define TOKEN               "wjaIvyw2ADIBlDEo4MEl"
#define THINGSBOARD_SERVER  "thingsboard.cloud"


// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

const int DHT_PIN = 15;

DHTesp dhtSensor;

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  Serial.println();
  pinMode(pinRelay, OUTPUT);
  // InitWiFi();
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  Serial.println("Sending data...");

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  tb.sendTelemetryInt("temperature", data.temperature);
  tb.sendTelemetryFloat("humidity", data.humidity);
  Serial.print("Suhu : ");
  Serial.print(data.temperature);
  Serial.print(" Kelembaban : ");
  Serial.println(data.humidity);
  if(data.temperature < 28){
   digitalWrite(pinRelay, HIGH);
  } else{
    digitalWrite(pinRelay, LOW);
  }

  tb.loop();

  tb.loop();
}