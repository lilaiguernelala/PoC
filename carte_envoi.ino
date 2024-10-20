#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#define WLAN_SSID "Co matteo"
#define WLAN_PASS "matteo2712"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883  // use 8883 for SSL
#define AIO_USERNAME "matteo_brunet"
#define AIO_KEY "aio_kbEl4257MEP2EjyusXcJhk3FTpRk"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// Setup a feed called 'photocell' for publishing.
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish presence = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/presence");
void MQTT_connect();

Adafruit_BME680 bme;

const int Infra_sensor_OUTPUT_PIN = 4;

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

  while(!Serial);
  Serial.println(F("BME680 test"));
 
  if (!bme.begin())
  {
    Serial.println("capteur non trouvé, vérifiez le cablage");
    while(1);
  }

  bme.setTemperatureOversampling(BME680_OS_8X);

  pinMode(Infra_sensor_OUTPUT_PIN, INPUT);
}

void loop() {
  MQTT_connect();
  test_lecture_bme680();
  Serial.print(F("\nSending temperature val "));
  Serial.print(bme.temperature);
  Serial.print(" °C");
  Serial.print("...");
  if (!temperature.publish(bme.temperature)) 
  {
    Serial.println(F("Failed"));
  } 
  else 
  {
    Serial.println(F("OK!"));
  }
  Serial.print(F("\nSending presence state "));
  Serial.print(digitalRead(Infra_sensor_OUTPUT_PIN));
  Serial.print("...");
  if (!presence.publish((uint32_t)digitalRead(Infra_sensor_OUTPUT_PIN))) 
  {
    Serial.println(F("Failed"));
  } 
  else 
  {
    Serial.println(F("OK!"));
  }
  delay(5000);
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

void test_lecture_bme680()
{
  if (! bme.performReading()) {
  Serial.println("Failed to perform reading :(");
  return;
  }
}
delay(1000);