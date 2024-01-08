#include <WiFi.h>
#include <ArduinoJson.h>
#include <NeoPixelBus.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include "config.h"

// Replace with your network credentials
const char *ssid = SSID;
const char *password = PASSWORD;

// Shelly 3EM device IP address
const char *shellyIP = SHELLY_IP;

// Create an instance of the NeoPixelBus library
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, LED_PIN);

int last_leds = 0;

void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial attach
  Serial.println();
  Serial.print("Initializing light strip...");
  strip.Begin();
  strip.Show();
  Serial.println("finished.");

  Serial.print("Setting up WiFi...");
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("finished.");

  // Enable OTA updates
  ArduinoOTA.setHostname("ledpowermeter");
  ArduinoOTA.begin();
  digitalWrite(LED, HIGH);
}

RgbColor getColorFromPower(bool producing)
{
  if (producing)
    return RgbColor(0, 1, 0);
  return RgbColor(1, 0, 0);
}

void loop()
{
  // Handle OTA update requests
  ArduinoOTA.handle();

  // Make an HTTP request to the Shelly 3EM device API to get the current power consumption data
  String url = "http://" + String(shellyIP) + "/status";
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == 200)
  {
    // Parse the response data and extract the total power consumption value
    String payload = http.getString();

    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);
    float power = doc["total_power"];

    Serial.print("Power: ");
    Serial.println(power);

    // Display the power consumption value on the LED matrix
    int powerDisplay = (int)power;

    bool producing = power < 0;

    if (producing)
      power = -power;
    if (power > MAX_POWER)
      power = MAX_POWER;
    int step = (producing ? MAX_PRODUCING : MAX_POWER) / LED_COUNT;
    int target_leds = power / step;
    while (last_leds != target_leds)
    {
      if(target_leds > last_leds) last_leds++;
      if(target_leds < last_leds) last_leds--;
      strip.ClearTo(0);
      for (int i = 0; i < last_leds; i++)
      {
        strip.SetPixelColor(i, getColorFromPower(producing));
      }
      strip.Show();
      delay(ANIMATE_DELAY);
    }
  }
  else
  {
    digitalWrite(LED, LOW);
    Serial.println("Error making HTTP request");
  }

  // Wait for a second before making the next request
  delay(POLLING_DELAY);
  digitalWrite(LED, HIGH);
}
