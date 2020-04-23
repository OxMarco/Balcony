/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include "BMP280.h"
#include "params.h"

#define DEBUG

unsigned long int sleep_time = 10 * 60e6; // 10 minutes
unsigned long int err_sleep_time = 60e6; // 1 minute

BMP280 bmp;
const int min_temp = -4;
const int max_temp = 50;
const int min_pres = 800;
const int max_pres = 2000;

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.setTimeout(2000);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif
  
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

  bmp.setSampling(BMP280::MODE_NORMAL,      /* Operating Mode. */
                  BMP280::SAMPLING_X2,      /* Temp. oversampling */
                  BMP280::SAMPLING_X16,     /* Pressure oversampling */
                  BMP280::FILTER_X16,       /* Filtering. */
                  BMP280::STANDBY_MS_1000); /* Standby time. */
#ifdef DEBUG
  Serial.println("setup concluded");
#endif
}

void loop() {
  delay(5000);
  unsigned long int t;

#ifdef DEBUG
  Serial.print("connecting to ");
  Serial.println(host);
#endif
  
  float temp = bmp.readTemperature();
  float pres = bmp.readPressure();
 
#ifdef DEBUG
  Serial.println("temp");
  Serial.println(temp);
  Serial.println("pres");
  Serial.println(pres);
#endif
  String url = host;

  if( (temp < min_temp || temp > max_temp) || (pres < min_pres || pres > max_pres) )
  {
    url += "field8=300";
    t = err_sleep_time;
  } 
  else
  {
    url += "field2=";
    url += temp;
    url += "field3=";
    url += pres;
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

  bmp.setSampling(BMP280::MODE_SLEEP);

  ESP.deepSleep(t);
}
