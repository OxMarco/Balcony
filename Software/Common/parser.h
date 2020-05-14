/**
 * @brief Sensor box data (de)serialisation functions
 */
char* serializeSensorData(float pressure,
                float temperature,
                float humidity,
                float light,
                float moisture) {
  DynamicJsonDocument json(500);
  char buff[1024];

  json["pressure"] = pressure;
  json["temperature"] = temperature;
  json["humidity"] = humidity;
  json["light"] = light;
  json["moisture"] = moisture;
  json["tmsp"] = timeClient.getEpochTime();
   
  serializeJson(json, buff);

  return buff;
}

void deserializeSensorData(String json) {
  DynamicJsonDocument buff(500);
  DeserializationError err = deserializeJson(buff,json);

  float pressure = buff['pressure'];
  float temperature = buff['temperature'];
  float humidity = buff['humidity'];
  float light = buff['light'];
  float moisture = buff['moisture'];
  long tmsp = buff['tmsp'];
}

/**
 * @brief Hub data (de)serialisation functions
*/

char* serializeHubData(float air_quality,
                float temperature,
                float humidity,
                bool valveStatus) {
  DynamicJsonDocument json(500);
  char buff[1024];

  json["air_quality"] = air_quality;
  json["temperature"] = temperature;
  json["humidity"] = humidity;
  json["valveStatus"] = valveStatus;
  json["tmsp"] = timeClient.getEpochTime();
   
  serializeJson(json, buff);

  return buff;
}

void deserializeHubData(String json) {
  DynamicJsonDocument buff(500);
  DeserializationError err = deserializeJson(buff,json);

  float air_quality = buff['air_quality'];
  float temperature = buff['temperature'];
  float humidity = buff['humidity'];
  bool valveStatus = buff['valveStatus'];
  long tmsp = buff['tmsp'];
}
