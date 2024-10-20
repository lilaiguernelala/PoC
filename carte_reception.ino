#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID "Co matteo"
#define WLAN_PASS "matteo2712"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883  // use 8883 for SSL
#define AIO_USERNAME "matteo_brunet"
#define AIO_KEY "aio_kbEl4257MEP2EjyusXcJhk3FTpRk"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
//setup la reception
Adafruit_MQTT_Subscribe temperature = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Subscribe presence = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/presence");
void MQTT_connect();

int pinventilo = 4;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

mqtt.subscribe(&temperature);
mqtt.subscribe(&presence);

pinMode(pinventilo, OUTPUT);
}

void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  subscription = mqtt.readSubscription(5000);
  if (subscription == &temperature) {
    Serial.print(F("Got: "));
    Serial.println((char *)temperature.lastread);
  }
  if (subscription == &presence) {
    Serial.print(F("Got: "));
    Serial.println((char *)presence.lastread);
  }
  String temp((char*)temperature.lastread); 
  String temp2((char*)presence.lastread); 
  float val_temp = temp.toFloat();
  int val_pres = temp2.toInt();
  if (val_temp >= 27 && val_pres != 0)
  {
    digitalWrite(pinventilo, HIGH);
  }
  else
  {
    digitalWrite(pinventilo, LOW);
  }
}

void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
}
