/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <coredecls.h>
#include <include/WiFiState.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "MAX44009.h"
#include "params.h"
#include "ota.h"

#define DEBUG

ADC_MODE(ADC_VCC);

unsigned long int sleep_time = 10 * 60e6; // 10 minutes
unsigned long int err_sleep_time = 60e6; // 1 minute

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 7200;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

MAX44009 Lux(0x4A);
const int min_lux = 0;
const int max_lux = 18800;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

#ifdef DEBUG
  Serial.begin(115200);
  Serial.setTimeout(2000);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif

  // Setup WiFi
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long wifiConnectStart = millis();
  
  while (WiFi.status() != WL_CONNECTED) {
    // Check to see if
    if (WiFi.status() == WL_CONNECT_FAILED) {
#ifdef DEBUG
      Serial.println("Failed to connect to WiFi. Please verify credentials: ");
#endif
      delay(10000);
    }

    delay(500);
#ifdef DEBUG
    Serial.println("...");
#endif
    // Only try for 5 seconds.
    if (millis() - wifiConnectStart > 15000) {
#ifdef DEBUG
      Serial.println("Failed to connect to WiFi");
#endif
      ESP.deepSleep(err_sleep_time);
    }
  }

#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif

  // Setup NTP
  timeClient.begin();

  // Setup MQTT

  // Setup sensors
  Lux.Begin(min_lux, max_lux); //Begin with full range min and max values

#ifdef DEBUG
  Serial.println("setup concluded");
#endif
}

void loop() {
  timeClient.update();

  unsigned long int t;

#ifdef DEBUG
  Serial.print("connecting to ");
  Serial.println(host);
#endif
  
  float lux = Lux.GetLux();

#ifdef DEBUG
  Serial.println("lux");
  Serial.println(lux);
#endif
  String url = host;

  if(lux < min_lux || lux > max_lux)
  {
    url += "field8=1";
    t = err_sleep_time;
  } 
  else
  {
    url += "field1=";
    url += lux;
    t = sleep_time;
  }

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();

  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
#ifdef DEBUG
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
#endif

    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
#ifdef DEBUG
      Serial.println(payload);
      Serial.println("sleep time");
      Serial.println(t);
#endif
    }
  } else {
#ifdef DEBUG
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
#endif
  }

  http.end();

#ifdef DEBUG
  Serial.println();
  Serial.println("closing connection");
#endif

  ESP.deepSleep(t);
}
