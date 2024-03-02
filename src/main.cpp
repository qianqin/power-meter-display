#include <WiFi.h>
#include <ArduinoJson.h>
#include <NeoPixelBus.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include "config.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Gree.h>

// Replace with your network credentials
const char *ssid = SSID;
const char *password = PASSWORD;

// Shelly 3EM device IP address
const char *shellyIP = SHELLY_IP;

IRGreeAC ac(GREE_AC_PIN, GREE_AC_REMOTE, false, true);

// Create an instance of the NeoPixelBus library
NeoPixelBus<NeoGrbFeature, NEOPIXEL_METHOD> strip(LED_COUNT, LED_PIN);

int current_leds = 0;
unsigned long last_poll = 0;
unsigned long last_animate = 0;
unsigned long last_change = 0;
int target[LED_COUNT];
int current[LED_COUNT];
bool producing = false;
int power = 0;

int get_power();
void ac_on();
void ac_off();

void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  ac.begin();
  ac.calibrate();
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

  for (int i = 0; i < LED_COUNT; i++)
  {
    current[i] = 0;
    target[i] = 0;
  }
  ac_on();
  digitalWrite(LED, HIGH);
}

RgbColor getColorWithPower(byte value)
{
  if (producing)
    return RgbColor(0, value, 0);
  return RgbColor(value, 0, 0);
}

void loop()
{
  // Handle OTA update requests
  ArduinoOTA.handle();

  unsigned long now = millis();

  if ((unsigned long)(now - last_animate) > ANIMATE_DELAY)
  {
    bool hasChanged = false;
    // time to do animation
    strip.ClearTo(0);
    for (int i = 0; i < LED_COUNT; i++)
    {
      if (current[i] == target[i])
      {
        strip.SetPixelColor(i, getColorWithPower(target[i]));
        continue;
      }
      if (current[i] < target[i])
        current[i] += ANIMATE_STEP;
      if (current[i] > target[i])
        current[i] -= ANIMATE_STEP;
      if (current[i] > 255)
        current[i] = 255;
      if (current[i] < 0)
        current[i] = 0;
      strip.SetPixelColor(i, getColorWithPower(current[i]));
      hasChanged = true;
    }
    if (hasChanged)
      strip.Show();
    last_animate = now;
  }

  if ((unsigned long)(now - last_poll) > POLLING_DELAY)
  {
    // time to poll data
    power = get_power();
    last_poll = now;
  }

  if ((unsigned long)(now - last_change) > LED_CHANGE_DELAY)
  {
    // time to poll data
    producing = power < 0;

    if (producing)
      power = -power;
    if (producing && power > MAX_POWER)
      power = MAX_POWER;
    if (!producing && power > MAX_PRODUCING)
      power = MAX_PRODUCING;
    int step = (producing ? MAX_PRODUCING : MAX_POWER) / LED_COUNT;
    int target_leds = power / step;

    if (target_leds > current_leds)
      current_leds++;
    if (target_leds < current_leds)
      current_leds--;

    for (int i = 0; i < LED_COUNT; i++)
    {
      if (i < current_leds)
      {
        target[i] = producing ? LED_MAX_BRIGHTNESS_PRODUCING : LED_MAX_BRIGHTNESS_CONSUMING;
      }
      else
      {
        target[i] = 0;
      }
    }
    last_change = now;
  }
}

int get_power()
{
  digitalWrite(LED, HIGH);
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
    return (int)power;
  }
  else
  {
    digitalWrite(LED, LOW);
    Serial.println("Error making HTTP request");
  }
  return 0;
}

void ac_on()
{
  ac.on();
  ac.setFan(GREE_AC_FAN);
  ac.setMode(GREE_AC_MODE);
  ac.setTemp(GREE_AC_TEMP);
  ac.setSwingVertical(true, kGreeSwingAuto);
  ac.setXFan(false);
  ac.setLight(GREE_AC_LED);
  ac.setSleep(false);
  ac.setTurbo(false);
  ac.send(GREE_AC_SEND_REPEAT);
}

void ac_off()
{
  ac.off();
  ac.send(GREE_AC_SEND_REPEAT);
}