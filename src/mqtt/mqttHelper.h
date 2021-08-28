#ifndef MQTTHELPER_h
#define MQTTHELPER_h

void setupMqtt(void (*subscriptionCallback)());
void mqttLoop();
void publishToMqtt(const char *topic, const char *payload);
void publishToMqttSubTopic(const char *subTopic, const char *payload);

#endif