#define MAX_CONN_TRIES 10

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_id = "";
const int mqtt_qos = 2;
const bool mqtt_retain = true;

// topics
const char* cmd_topic = "cmd";
const char* bme280_topic = "sensor/bme280";
const char* max44009_topic = "sensor/max44009";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
}

void setupMqtt()
{
    client.setServer(mqtt_server, mqtt_port);
    client.subscribe(cmd_topic);
    client.setCallback(callback);
}

void reconnectMqtt()
{
    int n = 0;
    
    // Loop until we're reconnected
    while (!client.connected() && n < MAX_CONN_TRIES) {
        Logger.info("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(mqtt_id)) {
            Logger.info("connected");
        } else {
            Logger.warn("failed, rc=");
            Logger.warn(client.state());
            Logger.warn(" try again in 5 seconds");
            n++;
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void loopMqtt()
{
    if (!client.connected()) {
        reconnectMqtt();
    }
    client.loop();
}

void sendMqtt(String msg)
{
    client.publish("balcony/box", msg, mqtt_qos, mqtt_retain);
}
