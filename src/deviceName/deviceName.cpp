#include "deviceName.h"
#include <WiFi.h>

namespace inkes
{
    DeviceName::DeviceName()
    {
        byte mac[6];
        WiFi.macAddress(mac);
        char macStr[18];

        sprintf(&macStr[0], "%x:%x:%x:%x:%x:%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        strncpy(&deviceName[8], macStr, 18);
    }

    char *DeviceName::get()
    {
        return &deviceName[0];
    }
}