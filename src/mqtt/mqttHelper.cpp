#include "mqttHelper.h"
#include "deviceName/deviceName.h"
#include "webConfiguration/webConf.h"
#include <PubSubClient.h>
#include <WiFi.h>

extern DeviceName deviceName;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
void (*savedSubscriptionCallback)();
bool isSetupDone = false;
mqttData data;
extern boolean isWifiConnected;

void onMqttMessage(char *topic, byte *payload, unsigned int length)
{
    char *payloadString = (char *)calloc(length + 1, sizeof(char));
    strncpy(payloadString, (char *)payload, length);
    *(payloadString + length) = 0;

    Serial.printf("MQTT: %s (len: %d) - Payload: %s\n", topic, length, payloadString);

    free(payloadString);
}

char *buildFullTopicString(const char *subTopic)
{
    unsigned int length = strlen(data.topic) + strlen(subTopic);
    char *fullTopic = (char *)calloc(length + 1, sizeof(char));

    strncpy(fullTopic, data.topic, strlen(data.topic));
    strncpy(fullTopic + strlen(data.topic), subTopic, strlen(subTopic));
    *(fullTopic + length) = 0;

    return fullTopic;
}

void connectToMqtt(void (*subscriptionCallback)())
{
    boolean successFullyConnected = false;

    Serial.println("Attempting to connect to MQTT");

    while(!mqttClient.connected())
    {
        char *lastWill = buildFullTopicString("/status");

        if(strlen(data.username) > 0 && strlen(data.password) > 0)
            successFullyConnected = mqttClient.connect(deviceName.get(), data.username, data.password, lastWill, 2, false, "disconnected");
        else
            successFullyConnected = mqttClient.connect(deviceName.get(), lastWill, 2, false, "disconnected");

        free(lastWill);
    }

    if(successFullyConnected)
    {
        Serial.println("Connected to MQTT");
        subscriptionCallback();
        return;
    }

    Serial.println("Failed to connect to MQTT");
}

void setupMqtt(void (*subscriptionCallback)())
{
    savedSubscriptionCallback = subscriptionCallback;

    if(isSetupDone)
        return;

    data = getMqttData();

    mqttClient.setServer(data.server, data.port);
    mqttClient.setCallback(onMqttMessage);
    mqttClient.setKeepAlive(30);
    isSetupDone = true;
}

void mqttLoop()
{
    if (!isSetupDone)
        return;

    if(isWifiConnected && !mqttClient.connected())
        connectToMqtt(savedSubscriptionCallback);

    mqttClient.loop();
}

void publishToMqtt(const char *topic, const char *payload)
{
    if(!isWifiConnected || !mqttClient.connected())
    {
        Serial.printf("Failed to publish to MQTT:\n WiFi connected: %d\n MQTT connected: %d\n", isWifiConnected, mqttClient.connected());
        return;
    }

    mqttClient.publish(topic, payload);
}

void publishToMqttSubTopic(const char *subTopic, const char *payload)
{
    char *topic = buildFullTopicString(subTopic);
    publishToMqtt(topic, payload);
    free(topic);
}