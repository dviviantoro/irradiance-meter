#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

// #include <GS_SDHelper.h>
// #include <ESP_Google_Sheet_Client.h>

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

extern bool wifiState;
extern int ssidState;

void initWifi();
// void upload_to_gsheet(int value1, int value2, int value3);
void write_influx();