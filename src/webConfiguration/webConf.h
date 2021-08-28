#ifndef WEBCONF_H
#define WEBCONF_H
#include <WebServer.h>

#define STRING_LEN 128

struct mqttData
{
    char *server;
    int port;
    char *username;
    char *password;
    char *topic;
};

void configureIotWebConf(WebServer::THandlerFunction configSavedCallback, WebServer::THandlerFunction wifiConnectedCallback);

mqttData getMqttData();

#endif;