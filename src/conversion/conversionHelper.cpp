#include "conversionHelper.h"
#include <Arduino.h>

int strToInt(char *str)
{
    char **conversionErrorPosition = 0;
    int convertedNumber = strtoul(str, conversionErrorPosition, 10);

    if (conversionErrorPosition == 0)
        return convertedNumber;

    Serial.printf("Error converting char* to number: %s\n", str);

    return -1;
}