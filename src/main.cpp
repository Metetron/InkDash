#include <Arduino.h>
#include "deviceName/deviceName.h"
#include "mqtt/mqttHelper.h"
#include "secrets.h"
#include "webConfiguration/webConf.h"
#include <DNSServer.h>
#include <IotWebConf.h>

DeviceName deviceName;
DNSServer dnsServer;
WebServer webServer(80);
IotWebConf webConf(deviceName.get(), &dnsServer, &webServer, INITIAL_ACCESSPOINT_PASSWORD);
bool isWifiConnected = false;

TaskHandle_t BackgroundTask;

void handleConfigSaved()
{
  Serial.println("Config saved");
  Serial.println("Device will reboot to use new settings...");
  publishToMqttSubTopic("/status", "Config changed");
  webConf.delay(1000);
  ESP.restart();
}

void handleWifiConnected()
{
  Serial.println("Connected to WiFi");
  isWifiConnected = true;
}

void connectedToMqtt()
{
  Serial.println("Connected to MQTT Server");
  publishToMqttSubTopic("/status", "connected");
}

void backgroundLoop(void *parameter)
{
  for(;;)
  {
    webConf.doLoop();
    setupMqtt(connectedToMqtt);
    mqttLoop();
  }
}

void setup() {
  Serial.begin(115200);
  configureIotWebConf(handleConfigSaved, handleWifiConnected);

  xTaskCreatePinnedToCore(
    backgroundLoop,
    "Background Loop",
    10000,
    NULL,
    0,
    &BackgroundTask,
    0
  );
}

void loop() {
}