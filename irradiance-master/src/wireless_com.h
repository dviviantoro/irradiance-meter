#include <WiFi.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <AsyncElegantOTA.h>
#include <WiFiClientSecure.h>
#include <ESPAsyncWebServer.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// #include <GS_SDHelper.h>
#include <ESP_Google_Sheet_Client.h>

extern bool wifiState;
extern int ssidState;

void initWifi();
void upload_to_gsheet(int value1, int value2, int value3);