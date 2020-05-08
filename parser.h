char* serialize(float pressure,
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

void deserialize(String json) {
  DynamicJsonDocument buff(500);
  DeserializationError err = deserializeJson(buff,json);

  float pressure = buff['pressure'];
  float temperature = buff['temperature'];
  float humidity = buff['humidity'];
  float light = buff['light'];
  float moisture = buff['moisture'];
  long tmsp = buff['tmsp'];
}
