#include  "WiFi.h" 
WiFiClient client;

#define ECHO_PIN 15
#define TRIG_PIN 2
#define ledMerah 13
#define ledKuning 12
#define ledHijau 14

String thingSpeakAddress = "api.thingspeak.com";
String writeAPIKey;
String tsfield1Name;
String request_string;

void setup() {
  //PIN
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ledMerah, OUTPUT);
  pinMode(ledKuning, OUTPUT);
  pinMode(ledHijau, OUTPUT);

  //WIFI
  WiFi.disconnect();
  WiFi.begin("Wokwi-GUEST", "");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

float readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  int duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void loop() {
  float distance = readDistanceCM();

  if(distance <= 100){
    digitalWrite(ledMerah, HIGH);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledHijau, LOW);
  }
  else if(distance >= 100 && distance <= 150){
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledKuning, HIGH);
    digitalWrite(ledHijau, LOW);
  }
  else{
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledHijau, HIGH);
  }
  delay(100);

  float waterHeight = 350 - distance;
  kirim_thingspeak(waterHeight);

}

void kirim_thingspeak(float distance) {
  if (client.connect("api.thingspeak.com", 80)) {
    request_string = "/update?";
    request_string += "key=";
    request_string += "S14MC7LL3SZBCQN8";
    request_string += "&";
    request_string += "field1";
    request_string += "=";
    request_string += distance;

    Serial.println(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n");
                 
    client.print(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

  }
}