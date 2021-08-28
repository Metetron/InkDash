#include "webConf.h"
#include "conversion\conversionHelper.h"
#include <IotWebConf.h>
#include <IotWebConfESP32HTTPUpdateServer.h>
#include <IotWebConfUsing.h>
#include <WebServer.h>

extern WebServer webServer;
extern IotWebConf webConf;

HTTPUpdateServer httpUpdater;

char mqttServer[STRING_LEN];
char mqttPort[6];
char mqttUsername[STRING_LEN];
char mqttPassword[STRING_LEN];
char mqttTopic[STRING_LEN];

IotWebConfParameterGroup mqttGroup = IotWebConfParameterGroup("mqttGroup", "MQTT Settings");
IotWebConfTextParameter mqttServerParam = IotWebConfTextParameter("MQTT Server", "mqttServer", mqttServer, STRING_LEN, "", "");
IotWebConfNumberParameter mqttPortParam = IotWebConfNumberParameter("MQTT Port", "mqttPort", mqttPort, 6, "1883");
IotWebConfTextParameter mqttUsernameParam = IotWebConfTextParameter("MQTT Username", "mqttUsername", mqttUsername, STRING_LEN, "", "");
IotWebConfPasswordParameter mqttPasswordParam = IotWebConfPasswordParameter("MQTT Password", "mqttPassword", mqttPassword, STRING_LEN, "", "");
IotWebConfTextParameter mqttTopicParam = IotWebConfTextParameter("MQTT Topic", "mqttTopic", mqttTopic, STRING_LEN, "", "");

void handleRoot();
void configureParameters();

void configureIotWebConf(WebServer::THandlerFunction configSavedCallback, WebServer::THandlerFunction wifiConnectedCallback)
{
    webConf.setConfigSavedCallback(configSavedCallback);
    webConf.setWifiConnectionCallback(wifiConnectedCallback);
    webConf.setupUpdateServer(
        [](const char *updatePath){httpUpdater.setup(&webServer, updatePath)}.
        [](const char *username, char *password){httpUpdater.updateCredentials(username, password);}
    );
    webConf.skipApStartup();

    configureParameters();

    webConf.init();

    webServer.on("/", handleRoot);
    webServer.on("/config", [] {webConf.handleConfig();});
    webServer.onNotFound([](){webConf.handleNotFound();});
}

mqttData getMqttData()
{
    mqttData data;

    data.server = mqttServer;
    data.port = strToInt(mqttPort);
    data.username = mqttUsername;
    data.password = mqttPassword;
    data.topic = mqttTopic;

    return data;
}

void configureParameters()
{
    mqttGroup.addItem(&mqttServerParam);
    mqttGroup.addItem(&mqttPortParam);
    mqttGroup.addItem(&mqttUsernameParam);
    mqttGroup.addItem(&mqttPasswordParam);
    mqttGroup.addItem(&mqttTopicParam);

    webConf.addParameterGroup(&mqttGroup);
}

void handleRoot()
{
    if (webConf.handleCaptivePortal())
        return;

    String html = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
    html += "<title>InkEs Update Server</title></head><body>";
    html += "Go to <a href='config'>configure page</a> to change values.";
    html += "</body></html>\n";

    webServer.send(200, "text/html", html);
}