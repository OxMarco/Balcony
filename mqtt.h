const char* mqtt_server = "192.168.1.50";
const char* mqtt_user = "janedoe";
const char* mqtt_password = "johndoe"
const int mqtt_port = 1883;

const char* cmd_topic = "cmd";
const char* bme280_topic = "";

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, mqtt_server, mqtt_port, mqtt_user, mqtt_password);

Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/box1/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

