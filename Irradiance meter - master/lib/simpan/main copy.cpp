#include <Arduino.h>
#include "RTClib.h"
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <WiFi.h>
#include "Nextion.h"
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <Adafruit_AHTX0.h>
#include <AsyncElegantOTA.h>
#include <WiFiClientSecure.h>
#include <ESPAsyncWebServer.h>
#include <UniversalTelegramBot.h>

int preset1 = 5000;
int preset2 = 20000;
int preset3 = 30000;
int preset4 = 40000;
int preset5 = 50000;
int preset6 = 60000;
int preset7 = 70000;
int preset8 = 80000;
#define timeWait 3000

int start1 = 4;
int start2 = 5;
int start3 = 6;
int start4 = 7;
int start5 = 8;
int end1 = 16;
int end2 = 17;
int end3 = 18;
int end4 = 19;
int end5 = 20;

int slcStart = start3;
int slcEnd = end3;

int rtcState = 0;

const int ledPin = 2;
bool ledState = LOW;

unsigned long previousMillis = 0;
unsigned long interval = 10000;

int dummy = 0;
String dataMessage, payload, filePath;

byte newCharSquare[8] = {
  B01100,
  B10010,
  B00100,
  B01000,
  B11110,
  B00000,
  B00000,
  B00000
};
 
unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long time_3 = 0;
unsigned long time_4 = 0;

int currentPage = 0;
int ssidState = 0;
int slcDev1 = preset6;
int slcDev2 = preset7;
int slcDev3 = preset8;

const int adc0 = 36;
const int adc1 = 26;
const int relay0 = 35;
int adc0Val = 0;
int adc1Val = 0;

int relay0State = 0;
int mainPowerState = 0;
int batPowerState = 0;
int sdCardState = 0;
int state1 = 0, state2 = 0, state3 = 0, stateMaster = 0;

const char* KNOWN_SSID[] = {"Galaxy-Deny", "INFORMATION CENTER", "TP-Link_5FBA", "smpitwasilah"};
const char* KNOWN_PASSWORD[] = {"bismillah", "9000000000", "68043636", "cipanas15"};
const int   KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); // number of known networks

const char* resource = "/trigger/irradiance/with/key/jto3ra7Ajzi5n2hr_SZ0bCBBKSjmDb-AQ5YRjlC0_Il";

#define SD_CS 5
#define RXD2 16
#define TXD2 17
// #define BOTtoken "1737474042:AAGk7scL0EVDONxnA5Tw5FfsCU2gLS8Egrk"  // your Bot Token (Get from Botfather)
#define BOTtoken "5917398507:AAFSCyGgvUH9LoeGT4Ch47d8yE3uMThglfw"  // your Bot Token (Get from Botfather)
// #define CHAT_ID "-465979089"    //group
#define CHAT_ID "-898094308"    //group irradiance
// #define CHAT_ID "691646481"   //saya

const char* serverIFTTT = "maker.ifttt.com";

RTC_DS1307 rtc;
Adafruit_AHTX0 aht;
WiFiClientSecure client;
AsyncWebServer server(80);
UniversalTelegramBot bot(BOTtoken, client);
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int value1 = 0, value2 = 0, value3 = 0;
float temp1 = 0, temp2 = 0, temp3 = 0;
float hum1 = 0, hum2 = 0, hum3 = 0;
int averageValue = 0;
const int Enable =  2;
int wifiState = 0;
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

String timestampDev1, timestampDev2, timestampDev3, timestampAvg;

// Declare your Nextion objects - Example (page id = 0, component id = 1, component name = "b0") 
// Page 0 = booting page
NexPage page0 = NexPage(0, 0, "page0");
NexText tLog0 = NexText(0, 1, "tLog0");
NexText tLog1 = NexText(0, 2, "tLog1");
NexText tLog2 = NexText(0, 3, "tLog2");
NexText tLog3 = NexText(0, 4, "tLog3");
NexText tLog4 = NexText(0, 5, "tLog4");
NexText tLog5 = NexText(0, 6, "tLog5");
NexText tLog6 = NexText(0, 7, "tLog6");
NexText tLog7 = NexText(0, 8, "tLog7");
NexText tLog8 = NexText(0, 9, "tLog8");
NexText tLog9 = NexText(0, 10, "tLog9");
NexText tLog10 = NexText(0, 11, "tLog10");
NexText tLog11 = NexText(0, 12, "tLog11");
NexText tLog12 = NexText(0, 13, "tLog12");
NexText tLog13 = NexText(0, 14, "tLog13");
NexText tLog14 = NexText(0, 15, "tLog14");
NexText tLog15 = NexText(0, 16, "tLog15");
NexText tLog16 = NexText(0, 17, "tLog15");
NexText tLog17 = NexText(0, 18, "tLog15");
NexText tLog18 = NexText(0, 19, "tLog15");
NexText tLog19 = NexText(0, 20, "tLog15");
NexText tLog20 = NexText(0, 21, "tLog15");
// Page 1 = home page
NexPage page1 = NexPage(1, 0, "page1");
NexPicture pDev1 = NexPicture(1, 6, "pDev1");
NexPicture pDev2 = NexPicture(1, 7, "pDev2");
NexPicture pDev3 = NexPicture(1, 8, "pDev3");
NexButton bDebug = NexButton(1, 1, "bDebug");
NexButton bKey = NexButton(1, 2, "bDKey");
NexButton bInfo = NexButton(1, 3, "bInfo");
NexText tValDev1 = NexText(1, 4, "tValDev1");
NexText tTimeDev1 = NexText(1, 5, "tTimeDev1");
NexText tSampling1 = NexText(1, 14, "tSampling1");
NexText tValDev2 = NexText(1, 9, "tValDev2");
NexText tTimeDev2 = NexText(1, 10, "tTimeDev2");
NexText tSampling2 = NexText(1, 15, "tSampling2");
NexText tValDev3 = NexText(1, 12, "tValDev3");
NexText tTimeDev3 = NexText(1, 11, "tTimeDev3");
NexText tSampling3 = NexText(1, 16, "tSampling3");
NexText tProgress = NexText(1, 13, "tProgress");
NexText tWifi = NexText(1, 19, "tWifi");
NexText tIP = NexText(1, 20, "tIP");
NexText tValAvg = NexText(1, 17, "tValAvg");
NexText tTimeAvg = NexText(1, 18, "tTimeAvg");
NexButton bTimeWork = NexButton(1, 22, "bTimeWork");
NexText tTimeWork = NexText(1, 23, "tTimeWork");
// Page 1 = config device1 page
NexPage page2 = NexPage(2, 0, "page2");
NexButton tPre1Dev1 = NexButton(2, 1, "tPre1Dev1");
NexButton tPre2Dev1 = NexButton(2, 2, "tPre2Dev1");
NexButton tPre3Dev1 = NexButton(2, 3, "tPre3Dev1");
NexButton tPre4Dev1 = NexButton(2, 4, "tPre4Dev1");
NexButton tPre5Dev1 = NexButton(2, 5, "tPre5Dev1");
NexButton tPre6Dev1 = NexButton(2, 6, "tPre6Dev1");
NexButton tPre7Dev1 = NexButton(2, 7, "tPre7Dev1");
NexButton tPre8Dev1 = NexButton(2, 8, "tPre8Dev1");
// Page 1 = config device2 page
NexPage page3 = NexPage(3, 0, "page3");
NexButton tPre1Dev2 = NexButton(3, 1, "tPre1Dev2");
NexButton tPre2Dev2 = NexButton(3, 2, "tPre2Dev2");
NexButton tPre3Dev2 = NexButton(3, 3, "tPre3Dev2");
NexButton tPre4Dev2 = NexButton(3, 4, "tPre4Dev2");
NexButton tPre5Dev2 = NexButton(3, 5, "tPre5Dev2");
NexButton tPre6Dev2 = NexButton(3, 6, "tPre6Dev2");
NexButton tPre7Dev2 = NexButton(3, 7, "tPre7Dev2");
NexButton tPre8Dev2 = NexButton(3, 8, "tPre8Dev2");
// Page 1 = config device3 page
NexPage page4 = NexPage(4, 0, "page4");
NexButton tPre1Dev3 = NexButton(4, 1, "tPre1Dev3");
NexButton tPre2Dev3 = NexButton(4, 2, "tPre2Dev3");
NexButton tPre3Dev3 = NexButton(4, 3, "tPre3Dev3");
NexButton tPre4Dev3 = NexButton(4, 4, "tPre4Dev3");
NexButton tPre5Dev3 = NexButton(4, 5, "tPre5Dev3");
NexButton tPre6Dev3 = NexButton(4, 6, "tPre6Dev3");
NexButton tPre7Dev3 = NexButton(4, 7, "tPre7Dev3");
NexButton tPre8Dev3 = NexButton(4, 8, "tPre8Dev3");
// Page 5 = info
NexPage page5 = NexPage(5, 0, "page5");
NexButton bInfoWA = NexButton(5, 1, "bInfoWA");
NexButton bInfoTele = NexButton(5, 2, "bInfoTele");
NexButton bInfoMail = NexButton(5, 3, "bInfoMail");
NexButton bInfoHome = NexButton(5, 4, "bInfoHome");
// Page contact me - whatsapp
NexPage page6 = NexPage(6, 0, "page6");
NexButton bInfoWAHome = NexButton(6, 1, "bInfoWAHome");
// Page contact me - telegram
NexPage page7 = NexPage(7, 0, "page7");
NexButton bInfoTeleHome = NexButton(7, 1, "bInfoTeleHome");
// Page contact me - mail
NexPage page8 = NexPage(8, 0, "page8");
NexButton bInfoMailHome = NexButton(8, 1, "bInfoMailHome");
// Page Key
NexPage page9 = NexPage(9, 0, "page9");
NexButton bKeyTele = NexButton(9, 1, "bKeyTele");
NexButton bKeyGraph = NexButton(9, 2, "bKeyGraph");
NexButton bKeySheet = NexButton(9, 3, "bKeySheet");
NexButton bKeyHome = NexButton(9, 4, "bHome5");
// Page key - telegram group
NexPage page10 = NexPage(10, 0, "page10");
NexButton bKeyTeleHome = NexButton(10, 1, "bKeyTeleHome");
// Page key - graph
NexPage page11 = NexPage(11, 0, "page11");
NexButton bKeyGraphHome = NexButton(11, 1, "bKeyGraphHome");
// Page key - spreadsheet
NexPage page12 = NexPage(12, 0, "page12");
NexButton bKeySheetHome = NexButton(12, 1, "bKeySheetHome");
// page time work
NexButton tStart1 = NexButton(13, 1, "tStart1");
NexButton tStart2 = NexButton(13, 2, "tStart2");
NexButton tStart3 = NexButton(13, 3, "tStart3");
NexButton tStart4 = NexButton(13, 4, "tStart4");
NexButton tStart5 = NexButton(13, 5, "tStart5");
NexButton tEnd1 = NexButton(13, 6, "tEnd1");
NexButton tEnd2 = NexButton(13, 7, "tEnd2");
NexButton tEnd3 = NexButton(13, 8, "tEnd3");
NexButton tEnd4 = NexButton(13, 9, "tEnd4");
NexButton tEnd5 = NexButton(13, 10, "tEnd5");

NexTouch *nex_listen_list[] = {
  &page0,
  &tLog0,
  &tLog1,
  &tLog2,
  &tLog3,
  &tLog4,
  &tLog5,
  &tLog6,
  &tLog7,
  &tLog8,
  &tLog9,
  &tLog10,
  &tLog11,
  &tLog12,
  &tLog13,
  &tLog14,
  &tLog15,
  &tLog16,
  &tLog17,
  &tLog18,
  &tLog19,
  &tLog20,

  &page1,
  &pDev1,
  &pDev2,
  &pDev3,
  &bDebug,
  &bKey,
  &bInfo,
  &bTimeWork,

  &page2,
  &tPre1Dev1,
  &tPre2Dev1,
  &tPre3Dev1,
  &tPre4Dev1,
  &tPre5Dev1,
  &tPre6Dev1,
  &tPre7Dev1,
  &tPre8Dev1,

  &page3,
  &tPre1Dev2,
  &tPre2Dev2,
  &tPre3Dev2,
  &tPre4Dev2,
  &tPre5Dev2,
  &tPre6Dev2,
  &tPre7Dev2,
  &tPre8Dev2,

  &page4,
  &tPre1Dev3,
  &tPre2Dev3,
  &tPre3Dev3,
  &tPre4Dev3,
  &tPre5Dev3,
  &tPre6Dev3,
  &tPre7Dev3,
  &tPre8Dev3,

  &bKeyTele,
  &bKeyGraph,
  &bKeySheet,
  &bKeyHome,
  &bKeyTeleHome,
  &bKeyGraphHome,
  &bKeySheetHome,

  &bInfoWA,
  &bInfoTele,
  &bInfoMail,
  &bInfoWAHome,
  &bInfoTeleHome,
  &bInfoMailHome,

  &tStart1,
  &tStart2,
  &tStart3,
  &tStart4,
  &tStart5,
  &tEnd1,
  &tEnd2,
  &tEnd3,
  &tEnd4,
  &tEnd5,

  &page5,
  &page6,
  &page7,
  &page8,
  &page9,
  &page10,
  &page11,
  &page12,
  NULL
};

/////////////////////////////////////////////////////////////////////////////////
void page0PopCallback(void *ptr) {currentPage = 1;}
void tLog0PopCallback(void *ptr) {currentPage = 1;}
void tLog1PopCallback(void *ptr) {currentPage = 1;}
void tLog2PopCallback(void *ptr) {currentPage = 1;}
void tLog3PopCallback(void *ptr) {currentPage = 1;}
void tLog4PopCallback(void *ptr) {currentPage = 1;}
void tLog5PopCallback(void *ptr) {currentPage = 1;}
void tLog6PopCallback(void *ptr) {currentPage = 1;}
void tLog7PopCallback(void *ptr) {currentPage = 1;}
void tLog8PopCallback(void *ptr) {currentPage = 1;}
void tLog9PopCallback(void *ptr) {currentPage = 1;}
void tLog10PopCallback(void *ptr) {currentPage = 1;}
void tLog11PopCallback(void *ptr) {currentPage = 1;}
void tLog12PopCallback(void *ptr) {currentPage = 1;}
void tLog13PopCallback(void *ptr) {currentPage = 1;}
void tLog14PopCallback(void *ptr) {currentPage = 1;}
void tLog15PopCallback(void *ptr) {currentPage = 1;}
void tLog16PopCallback(void *ptr) {currentPage = 1;}
void tLog17PopCallback(void *ptr) {currentPage = 1;}
void tLog18PopCallback(void *ptr) {currentPage = 1;}
void tLog19PopCallback(void *ptr) {currentPage = 1;}
void tLog20PopCallback(void *ptr) {currentPage = 1;}

void pDev1PopCallback(void *ptr) {currentPage = 2;}
void pDev2PopCallback(void *ptr) {currentPage = 3;}
void pDev3PopCallback(void *ptr) {currentPage = 4;}
void bDebugPopCallback(void *ptr) {currentPage = 0;}
void bKeyPopCallback(void *ptr) {currentPage = 5;}
void bKeyHomePopCallback(void *ptr) {currentPage = 1;}
void bKeyTelePopCallback(void *ptr) {currentPage = 6;}
void bKeyTeleHomePopCallback(void *ptr) {currentPage = 1;}
void bKeyGraphPopCallback(void *ptr) {currentPage = 7;}
void bKeyGraphHomePopCallback(void *ptr) {currentPage = 1;}
void bKeySheetPopCallback(void *ptr) {currentPage = 8;}
void bKeySheetHomePopCallback(void *ptr) {currentPage = 1;}
void bInfoPopCallback(void *ptr) {currentPage = 9;}
void bInfoHomePopCallback(void *ptr) {currentPage = 1;}
void bInfoWAPopCallback(void *ptr) {currentPage = 10;}
void bInfoWAHomePopCallback(void *ptr) {currentPage = 1;}
void bInfoTelePopCallback(void *ptr) {currentPage = 11;}
void bInfoTeleHomePopCallback(void *ptr) {currentPage = 1;}
void bInfoMailPopCallback(void *ptr) {currentPage = 12;}
void bInfoMailHomePopCallback(void *ptr) {currentPage = 1;}
void bTimeWorkPopCallback(void *ptr) {currentPage = 13;}

void tPre1Dev1PopCallback(void *ptr) {slcDev1 = preset1;currentPage = 1;}
void tPre2Dev1PopCallback(void *ptr) {slcDev1 = preset2;currentPage = 1;}
void tPre3Dev1PopCallback(void *ptr) {slcDev1 = preset3;currentPage = 1;}
void tPre4Dev1PopCallback(void *ptr) {slcDev1 = preset4;currentPage = 1;}
void tPre5Dev1PopCallback(void *ptr) {slcDev1 = preset5;currentPage = 1;}
void tPre6Dev1PopCallback(void *ptr) {slcDev1 = preset6;currentPage = 1;}
void tPre7Dev1PopCallback(void *ptr) {slcDev1 = preset7;currentPage = 1;}
void tPre8Dev1PopCallback(void *ptr) {slcDev1 = preset8;currentPage = 1;}

void tPre1Dev2PopCallback(void *ptr) {slcDev2 = preset1;currentPage = 1;}
void tPre2Dev2PopCallback(void *ptr) {slcDev2 = preset2;currentPage = 1;}
void tPre3Dev2PopCallback(void *ptr) {slcDev2 = preset3;currentPage = 1;}
void tPre4Dev2PopCallback(void *ptr) {slcDev2 = preset4;currentPage = 1;}
void tPre5Dev2PopCallback(void *ptr) {slcDev2 = preset5;currentPage = 1;}
void tPre6Dev2PopCallback(void *ptr) {slcDev2 = preset6;currentPage = 1;}
void tPre7Dev2PopCallback(void *ptr) {slcDev2 = preset7;currentPage = 1;}
void tPre8Dev2PopCallback(void *ptr) {slcDev2 = preset8;currentPage = 1;}

void tPre1Dev3PopCallback(void *ptr) {slcDev3 = preset1;currentPage = 1;}
void tPre2Dev3PopCallback(void *ptr) {slcDev3 = preset2;currentPage = 1;}
void tPre3Dev3PopCallback(void *ptr) {slcDev3 = preset3;currentPage = 1;}
void tPre4Dev3PopCallback(void *ptr) {slcDev3 = preset4;currentPage = 1;}
void tPre5Dev3PopCallback(void *ptr) {slcDev3 = preset5;currentPage = 1;}
void tPre6Dev3PopCallback(void *ptr) {slcDev3 = preset6;currentPage = 1;}
void tPre7Dev3PopCallback(void *ptr) {slcDev3 = preset7;currentPage = 1;}
void tPre8Dev3PopCallback(void *ptr) {slcDev3 = preset8;currentPage = 1;}

void tStart1PopCallback(void *ptr) {slcStart = start1;currentPage = 1;}
void tStart2PopCallback(void *ptr) {slcStart = start2;currentPage = 1;}
void tStart3PopCallback(void *ptr) {slcStart = start3;currentPage = 1;}
void tStart4PopCallback(void *ptr) {slcStart = start4;currentPage = 1;}
void tStart5PopCallback(void *ptr) {slcStart = start5;currentPage = 1;}
void tEnd1PopCallback(void *ptr) {slcEnd = end1;currentPage = 1;}
void tEnd2PopCallback(void *ptr) {slcEnd = end2;currentPage = 1;}
void tEnd3PopCallback(void *ptr) {slcEnd = end3;currentPage = 1;}
void tEnd4PopCallback(void *ptr) {slcEnd = end4;currentPage = 1;}
void tEnd5PopCallback(void *ptr) {slcEnd = end5;currentPage = 1;}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void page0PushCallback(void *ptr) {currentPage = 1;}
void tLog0PushCallback(void *ptr) {currentPage = 1;}
void tLog1PushCallback(void *ptr) {currentPage = 1;}
void tLog2PushCallback(void *ptr) {currentPage = 1;}
void tLog3PushCallback(void *ptr) {currentPage = 1;}
void tLog4PushCallback(void *ptr) {currentPage = 1;}
void tLog5PushCallback(void *ptr) {currentPage = 1;}
void tLog6PushCallback(void *ptr) {currentPage = 1;}
void tLog7PushCallback(void *ptr) {currentPage = 1;}
void tLog8PushCallback(void *ptr) {currentPage = 1;}
void tLog9PushCallback(void *ptr) {currentPage = 1;}
void tLog10PushCallback(void *ptr) {currentPage = 1;}
void tLog11PushCallback(void *ptr) {currentPage = 1;}
void tLog12PushCallback(void *ptr) {currentPage = 1;}
void tLog13PushCallback(void *ptr) {currentPage = 1;}
void tLog14PushCallback(void *ptr) {currentPage = 1;}
void tLog15PushCallback(void *ptr) {currentPage = 1;}
void tLog16PushCallback(void *ptr) {currentPage = 1;}
void tLog17PushCallback(void *ptr) {currentPage = 1;}
void tLog18PushCallback(void *ptr) {currentPage = 1;}
void tLog19PushCallback(void *ptr) {currentPage = 1;}
void tLog20PushCallback(void *ptr) {currentPage = 1;}

void pDev1PushCallback(void *ptr) {currentPage = 2;}
void pDev2PushCallback(void *ptr) {currentPage = 3;}
void pDev3PushCallback(void *ptr) {currentPage = 4;}
void bDebugPushCallback(void *ptr) {currentPage = 0;}
void bKeyPushCallback(void *ptr) {currentPage = 5;}
void bKeyHomePushCallback(void *ptr) {currentPage = 1;}
void bKeyTelePushCallback(void *ptr) {currentPage = 6;}
void bKeyTeleHomePushCallback(void *ptr) {currentPage = 1;}
void bKeyGraphPushCallback(void *ptr) {currentPage = 7;}
void bKeyGraphHomePushCallback(void *ptr) {currentPage = 1;}
void bKeySheetPushCallback(void *ptr) {currentPage = 8;}
void bKeySheetHomePushCallback(void *ptr) {currentPage = 1;}
void bInfoPushCallback(void *ptr) {currentPage = 9;}
void bInfoHomePushCallback(void *ptr) {currentPage = 1;}
void bInfoWAPushCallback(void *ptr) {currentPage = 10;}
void bInfoWAHomePushCallback(void *ptr) {currentPage = 1;}
void bInfoTelePushCallback(void *ptr) {currentPage = 11;}
void bInfoTeleHomePushCallback(void *ptr) {currentPage = 1;}
void bInfoMailPushCallback(void *ptr) {currentPage = 12;}
void bInfoMailHomePushCallback(void *ptr) {currentPage = 1;}
void bTimeWorkPushCallback(void *ptr) {currentPage = 13;}

void tPre1Dev1PushCallback(void *ptr) {slcDev1 = preset1;currentPage = 1;}
void tPre2Dev1PushCallback(void *ptr) {slcDev1 = preset2;currentPage = 1;}
void tPre3Dev1PushCallback(void *ptr) {slcDev1 = preset3;currentPage = 1;}
void tPre4Dev1PushCallback(void *ptr) {slcDev1 = preset4;currentPage = 1;}
void tPre5Dev1PushCallback(void *ptr) {slcDev1 = preset5;currentPage = 1;}
void tPre6Dev1PushCallback(void *ptr) {slcDev1 = preset6;currentPage = 1;}
void tPre7Dev1PushCallback(void *ptr) {slcDev1 = preset7;currentPage = 1;}
void tPre8Dev1PushCallback(void *ptr) {slcDev1 = preset8;currentPage = 1;}

void tPre1Dev2PushCallback(void *ptr) {slcDev2 = preset1;currentPage = 1;}
void tPre2Dev2PushCallback(void *ptr) {slcDev2 = preset2;currentPage = 1;}
void tPre3Dev2PushCallback(void *ptr) {slcDev2 = preset3;currentPage = 1;}
void tPre4Dev2PushCallback(void *ptr) {slcDev2 = preset4;currentPage = 1;}
void tPre5Dev2PushCallback(void *ptr) {slcDev2 = preset5;currentPage = 1;}
void tPre6Dev2PushCallback(void *ptr) {slcDev2 = preset6;currentPage = 1;}
void tPre7Dev2PushCallback(void *ptr) {slcDev2 = preset7;currentPage = 1;}
void tPre8Dev2PushCallback(void *ptr) {slcDev2 = preset8;currentPage = 1;}

void tPre1Dev3PushCallback(void *ptr) {slcDev3 = preset1;currentPage = 1;}
void tPre2Dev3PushCallback(void *ptr) {slcDev3 = preset2;currentPage = 1;}
void tPre3Dev3PushCallback(void *ptr) {slcDev3 = preset3;currentPage = 1;}
void tPre4Dev3PushCallback(void *ptr) {slcDev3 = preset4;currentPage = 1;}
void tPre5Dev3PushCallback(void *ptr) {slcDev3 = preset5;currentPage = 1;}
void tPre6Dev3PushCallback(void *ptr) {slcDev3 = preset6;currentPage = 1;}
void tPre7Dev3PushCallback(void *ptr) {slcDev3 = preset7;currentPage = 1;}
void tPre8Dev3PushCallback(void *ptr) {slcDev3 = preset8;currentPage = 1;}

void tStart1PushCallback(void *ptr) {slcStart = start1;currentPage = 1;}
void tStart2PushCallback(void *ptr) {slcStart = start2;currentPage = 1;}
void tStart3PushCallback(void *ptr) {slcStart = start3;currentPage = 1;}
void tStart4PushCallback(void *ptr) {slcStart = start4;currentPage = 1;}
void tStart5PushCallback(void *ptr) {slcStart = start5;currentPage = 1;}
void tEnd1PushCallback(void *ptr) {slcEnd = end1;currentPage = 1;}
void tEnd2PushCallback(void *ptr) {slcEnd = end2;currentPage = 1;}
void tEnd3PushCallback(void *ptr) {slcEnd = end3;currentPage = 1;}
void tEnd4PushCallback(void *ptr) {slcEnd = end4;currentPage = 1;}
void tEnd5PushCallback(void *ptr) {slcEnd = end5;currentPage = 1;}
/////////////////////////////////////////////////////////////////////////////////

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void logSDCard() {
  dataMessage = payload + "\r\n";
  appendFile(SD, filePath.c_str(), dataMessage.c_str());
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void printnextion() {
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void listen() {
  unsigned long startedWaiting = millis();
  while(Serial2.available() <= 0 && millis() - startedWaiting <= timeWait) {
    printnextion();
    Serial.print("tProgress.txt=\"");
    Serial.print("Listening for slave response . . .\""); 
    printnextion();

    String str = Serial2.readString();
    Serial.println(str);
    String part01 = getValue(str,';',0);
    String part02 = getValue(str,';',1);
    ////////////
    String part03 = getValue(str,';',2);
    String part04 = getValue(str,';',3);
    ////////////
    
    if (part01 == "rt1" && part02 == "1") {
      state1 = 1;
      break;
    } else if (part01 == "rt2" && part02 == "1") {
      state2 = 1;
      break;
    } else if (part01 == "rt3" && part02 == "1") {
      state3 = 1;
      break;
    }

    if (part01 == "ra1") {
      value1 = part02.toInt();
      temp1 = part03.toFloat();
      hum1 = part04.toFloat();
      break;
    } else if (part01 == "ra2") {
      value2 = part02.toInt();
      temp2 = part03.toFloat();
      hum2 = part04.toFloat();
      break;
    } else if (part01 == "ra3") {
      value3 = part02.toInt();
      temp3 = part03.toFloat();
      hum3 = part04.toFloat();
      break;
    }

    /*
    if (sdCardState == 1) {
      if (part01 == "ra1") {
        value1 = part02.toInt();
        Serial.print("tProgress.txt=\"");
        Serial.print("Saving to SD card . . .\"");
        printnextion();
        String payload = String(now.unixtime()) + String(";") + String(value1) + String(";") + String(value2) + String(";") + String(value3) + String(";");
        String filePath = String("/") + String(now.year(), DEC) + String(now.month(), DEC) + String(now.day(), DEC);
        writeFile(SD, filePath, payload);
        // writeFile(SD, "/logger.txt", payload);
        Serial.print("tProgress.txt=\"");
        Serial.print("Data saved to SD card (logger.txt)\"");
        printnextion();
        break;
      } else if (part01 == "ra2") {
        value2 = part02.toInt();
        break;
      } else if (part01 == "ra3") {
        value3 = part02.toInt();
        break;
      }
    }
    */
  }
  // delay(10);
}

void checkDevice1() {
  Serial2.print("t1");

  String str = Serial2.readString();
  Serial.println(str);
  String part01 = getValue(str,';',0);
  String part02 = getValue(str,';',1);
  String part03 = getValue(str,';',2);
  String part04 = getValue(str,';',3);
  if (part01 == "rt1" && part02 == "1") {
    state1 = 1;
  }
}

void checkDevice2() {
  Serial2.print("t2");
  listen();
  if (state2 == 0) {
    Serial2.print("t2");
    listen();
    if (state2 == 0) {
      Serial2.print("t2");
      listen();
    }
  }
}

void checkDevice3() {
  Serial2.print("t3");
  listen();
  if (state3 == 0) {
    Serial2.print("t3");
    listen();
    if (state3 == 0) {
      Serial2.print("t3");
      listen();
    }
  }
}

void checkPeripheral() {
  Serial.print("tLog1.txt=\"");
  Serial.print("Serial communication established\"");
  printnextion();
  delay(1000);

  // Serial.print("tLog.txt=\"");
  if (! aht.begin()) {
    Serial.println("AHT10 Error");
    // while (1);
  } else {
    Serial.println("AHT10 Initialized");
  }
  delay(1000);

  printnextion();
  Serial.print("tLog2.txt=\"");
  Serial.print("Initializing SD card . . .\"");
  printnextion();
  delay(1000);

  if (SD.begin()) {
    sdCardState = 1;
    Serial.print("tLog3.txt=\"");
    Serial.print("[OK] SD card mount success\"");
    printnextion();
  
    Serial.print("tLog4.txt=\"");
    uint8_t cardType = SD.cardType();
    if(cardType == CARD_NONE) {
      Serial.print("No SD card attached\"");
      return;
    }
    Serial.print("Initializing SD card type . . .\"");
    printnextion();

    Serial.print("tLog5.txt=\"");
    if(cardType == CARD_MMC){
      Serial.print("SD Card type is MMC\"");
    } else if(cardType == CARD_SD){
      Serial.print("SD Card type is SDSC\"");
    } else if(cardType == CARD_SDHC){
      Serial.print("SD Card type is SDHC\"");
    } else {
      Serial.print("SD Card type is unknown\"");
    }
    printnextion();
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.print("tLog6.txt=\"");
    Serial.printf("SD card size: %lluMB\n\"", cardSize);
    printnextion();
  } else if (!SD.begin()) {
    Serial.print("tLog3.txt=\"");
    Serial.print("[ERROR] SD card failed\"");
    printnextion();
  }
  delay(2000);

  // check slave-1 connection
  Serial.print("tLog7.txt=\"");
  Serial.print("Establishing modbus RS485 communication (Device-1) . . .\"");
  printnextion();
  Serial2.print("t1");
  listen();
  if (state1 == 0) {
    Serial2.print("t1");
    listen();
    if (state1 == 0) {
      Serial2.print("t1");
      listen();
    }
  }
  if (state1 == 1) {
    printnextion();
    Serial.print("tLog8.txt=\"");
    Serial.print("[OK] Slave-1 communication established\"");
    printnextion();
  } else {
    printnextion();
    Serial.print("tLog8.txt=\"");
    Serial.print("[ERROR] Slave-1 communication failed\"");
    printnextion();
  }
  delay(1000);

  // check slave-2 connection
  Serial.print("tLog9.txt=\"");
  Serial.print("Establishing modbus RS485 communication (Device-2) . . .\"");
  printnextion();
  Serial2.print("t2");
  listen();
  if (state2 == 0) {
    Serial2.print("t2");
    listen();
    if (state2 == 0) {
      Serial2.print("t2");
      listen();
    }
  }
  if (state2 == 1) {
    printnextion();
    Serial.print("tLog10.txt=\"");
    Serial.print("[OK] Slave-2 communication established\"");
    printnextion();
  } else {
    printnextion();
    Serial.print("tLog10.txt=\"");
    Serial.print("[ERROR] Slave-2 communication failed\"");
    printnextion();
  }
  delay(1000);

  // check slave-3 connection
  Serial.print("tLog11.txt=\"");
  Serial.print("Establishing modbus RS485 communication (Device-3) . . .\"");
  printnextion();
  Serial2.print("t3");
  listen();
  if (state3 == 0) {
    Serial2.print("t3");
    listen();
    if (state3 == 0) {
      Serial2.print("t3");
      listen();
    }
  }

  if (state3 == 1) {
    printnextion();
    Serial.print("tLog12.txt=\"");
    Serial.print("[OK] Slave-3 communication established\"");
    printnextion();
  } else {
    printnextion();
    Serial.print("tLog12.txt=\"");
    Serial.print("[ERROR] Slave-3 communication failed\"");
    printnextion();
  }
  delay(1000);

  // Check RTC
  Serial.print("tLog13.txt=\"");
  Serial.print("Establishing real time clock communication . . .\"");
  printnextion();
  rtcState = 1;
  Serial.print("tLog14.txt=\"");
  if (! rtc.begin()) {
    Serial.print("[ERROR] Real time clock communication\"");
    printnextion();
  }
  Serial.print("[OK] Real time clock communication\"");
  printnextion();

  if (! rtc.isrunning()) {
    // Serial2.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2023, 3, 16, 12, 59, 0));
  }

  
  // Check internet connection
}

void handleNewMessages(int numNewMessages) {
  DateTime now = rtc.now();

  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "This is telegram group service for Irradiance Meter IRM.V01 by Pesawat Sederhana\n\n";
      welcome += "Please use the following commands to control your outputs:\n";
      welcome += "/start to request command list \n";
      welcome += "/stat to request device status list \n";
      welcome += "/device1 to request information of device1 \n";
      welcome += "/device2 to request information of device2 \n";
      welcome += "/device3 to request information of device3 \n";
      welcome += "/master to request information of master device \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/stat") {
      String messageTelegram = "Status slave device: \n\n";
      if (state1 == 1) {
        messageTelegram += "Device1 is ONLINE\n";
      } else {
        messageTelegram += "Device1 is OFFLINE\n";
      }
      if (state2 == 1) {
        messageTelegram += "Device2 is ONLINE\n";
      } else {
        messageTelegram += "Device2 is OFFLINE\n";
      }
      if (state3 == 1) {
        messageTelegram += "Device3 is ONLINE\n";
      } else {
        messageTelegram += "Device3 is OFFLINE\n";
      }
      bot.sendMessage(chat_id, messageTelegram, "");
    }

    if (text == "/device1") {
      String messageTelegram = "Device1 current infomation: \n\n";
      if (state1 == 1) {
        messageTelegram += "Device1 is ONLINE\n";
        messageTelegram += "Sampling time = " + String(slcDev1) + " ms\n";
        messageTelegram += "Last value = " + String(value1) + " Watt/m^2 at " + String(timestampDev1) + "\n";
        messageTelegram += "Internal temperature = " + String(temp1) + " C\n";
        messageTelegram += "Internal RH = " + String(hum1) + " %\n";
      } else {
        messageTelegram += "Device1 is OFFLINE\n";
      }
      bot.sendMessage(chat_id, messageTelegram, "");
    }

    if (text == "/device2") {
      String messageTelegram = "Device2 current infomation: \n\n";
      if (state2 == 1) {
        messageTelegram += "Device2 is ONLINE\n";
        messageTelegram += "Sampling time = " + String(slcDev2) + " ms\n";
        messageTelegram += "Last value = " + String(value2) + " Watt/m^2 at " + String(timestampDev2) + "\n";
        messageTelegram += "Internal temperature = " + String(temp2) + " C\n";
        messageTelegram += "Internal RH = " + String(hum2) + " %\n";
      } else {
        messageTelegram += "Device2 is OFFLINE\n";
      }
      bot.sendMessage(chat_id, messageTelegram, "");
    }

    if (text == "/device3") {
      String messageTelegram = "Device3 current infomation: \n\n";
      if (state3 == 1) {
        messageTelegram += "Device3 is ONLINE\n";
        messageTelegram += "Sampling time = " + String(slcDev3) + " ms\n";
        messageTelegram += "Last value = " + String(value3) + " Watt/m^2 at " + String(timestampDev3) + "\n";
        messageTelegram += "Internal temperature = " + String(temp3) + " C\n";
        messageTelegram += "Internal RH = " + String(hum3) + " %\n";
      } else {
        messageTelegram += "Device3 is OFFLINE\n";
      }
      bot.sendMessage(chat_id, messageTelegram, "");
    }

    if (text == "/master") {
      sensors_event_t humidity, temp;
      aht.getEvent(&humidity, &temp);
      String messageTelegram = "Master device current infomation: \n\n";
      if (ssidState == 1) {
        messageTelegram += "Connected to Galaxy-Deny\n";
      } else if (ssidState == 2) {
        messageTelegram += "Connected to INFORMATION CENTER\n";
      } else if (ssidState == 3) {
        messageTelegram += "Connected to TP-Link_5FBA\n";
      } else if (ssidState == 4) {
        messageTelegram += "Connected to smpitwasilah\n";
      }
      // messageTelegram += "IP address: " + String(WiFi.localIP()) + "\n";
      messageTelegram += "Average value = " + String(averageValue) + " Watt/m^2 at " + String(timestampAvg) + "\n";
      messageTelegram += "Internal temperature = " + String(temp.temperature) + " C\n";
      messageTelegram += "Internal RH = " + String(humidity.relative_humidity) + " %\n";
      messageTelegram += "Woke up at " + String(slcStart) + ".00\n";
      messageTelegram += "Woke up at " + String(slcEnd) + ".00\n";
      bot.sendMessage(chat_id, messageTelegram, "");
    }
  }
}

void makeIFTTTRequest() {
  Serial.print("tProgress.txt=\"");
  Serial.print("Connecting to "); 
  Serial.print(serverIFTTT);
  Serial.print("\""); 
  printnextion();
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(serverIFTTT, 80) && (retries-- > 0)) {
    printnextion();
    Serial.print("tProgress.txt=\"");
    Serial2.print(". . . \"");
    printnextion();
  }
  if(!!!client.connected()) {
    printnextion();
    Serial.print("tProgress.txt=\"");
    Serial.print("Failed to connect...\"");
    printnextion();
  }
  
  printnextion();
  Serial.print("tProgress.txt=\"");
  Serial.print("Request resource: ");  
  Serial.print(resource);
  Serial.print("\""); 
  printnextion();

  // Temperature in Celsius
  String jsonObject = String("{\"value1\":\"") + value1 + "\",\"value2\":\"" + value2
                      + "\",\"value3\":\"" + value3 + "\"}";
                       
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + serverIFTTT); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    printnextion();
    Serial.print("tProgress.txt=\"");
    Serial.print("No response...\"");
    printnextion();
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  printnextion();
  Serial.print("tProgress.txt=\"");
  Serial.print("Closing connection\"");
  printnextion();
  client.stop(); 
}

void initWifi() {
  Serial.print("tLog15.txt=\"");
  Serial.print("Start scan Wi-Fi connection . . .\"");
  printnextion();

  boolean wifiFound = false;
  int i, n;

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");

  // WiFi.scanNetworks will return the number of networks found
  Serial.println(F("scan start"));
  int nbVisibleNetworks = WiFi.scanNetworks();
  Serial.println(F("scan done"));
  if (nbVisibleNetworks == 0) {
    Serial.println(F("no networks found. Reset to try again"));
    while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
  }

  // if you arrive here at least some networks are visible
  Serial.print(nbVisibleNetworks);
  Serial.println(" network(s) found");

  // check if we recognize one by comparing the visible networks one by one with our list of known networks
  for (i = 0; i < nbVisibleNetworks; ++i) {
    Serial.println(WiFi.SSID(i)); // Print current SSID
    for (n = 0; n < KNOWN_SSID_COUNT; n++) { // walk through the list of known SSID and check for a match
      if (strcmp(KNOWN_SSID[n], WiFi.SSID(i).c_str())) {
        Serial.print(F("\tNot matching "));
        Serial.println(KNOWN_SSID[n]);
      } else { // we got a match
        wifiFound = true;
        break; // n is the network index we found
      }
    } // end for each known wifi SSID
    if (wifiFound) break; // break from the "for each visible network" loop
  } // end for each visible network

  if (!wifiFound) {
    wifiState = 0;
    Serial.println(F("no Known network identified. Reset to try again"));

    printnextion();
    Serial.print("tLog16.txt=\"");
    Serial.print("[ERROR] No known network identified\"");
    printnextion();
    Serial.print("tLog17.txt=\"");
    Serial.print("[ERROR] No IP address\"");
    printnextion();
    Serial.print("tLog18.txt=\"");
    Serial.print("[ERROR] OTA not available\"");
    printnextion();
  } else {
    wifiState = 1;

    Serial.print(F("\nConnecting to "));
    Serial.println(KNOWN_SSID[n]);

    // We try to connect to the WiFi network we found
    WiFi.begin(KNOWN_SSID[n], KNOWN_PASSWORD[n]);
    
    if (KNOWN_SSID[n] == "Galaxy-Deny"){
      ssidState = 1;
    } else if (KNOWN_SSID[n] == "INFORMATION CENTER"){
      ssidState = 2;
    } else if (KNOWN_SSID[n] == "TP-Link_5FBA"){
      ssidState = 3;
    } else if (KNOWN_SSID[n] == "smpitwasilah"){
      ssidState = 4;
    } else {
      ssidState = 5;
    }

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial2.print(".");
    }
    Serial2.println("");
    
    printnextion();
    Serial.print("tLog16.txt=\"");
    Serial.print("[OK] Wi-Fi connected, your SSID is ");
    Serial.print(KNOWN_SSID[n]);
    Serial.print("\"");
    printnextion();

    // SUCCESS, you are connected to the known WiFi network
    Serial2.println(F("WiFi connected , your IP address is "));
    Serial2.println(WiFi.localIP());

    Serial.print("tLog17.txt=\"");
    Serial.print("[OK] Wi-Fi connected, your IP address is ");
    Serial.print(WiFi.localIP());
    Serial.print("\"");
    printnextion();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "Hi! I am master device!");
    });

    AsyncElegantOTA.begin(&server);
    server.begin();

    Serial.print("tLog18.txt=\"");
    Serial.print("[OK] OTA connected\"");
    printnextion();
  }

}

void device1(){
  DateTime now = rtc.now();
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  if(millis() >= time_1 + slcDev1){
    time_1 += slcDev1;
    timestampDev1 = String(now.year(), DEC) + String("/") + String(now.month(), DEC) + String("/") + String(now.day(), DEC) + String(", ") + String(now.hour(), DEC) + String(":") + String(now.minute(), DEC) + String(":") + String(now.second(), DEC);
    timestampAvg = timestampDev1;
    printnextion();
    Serial.print("tProgress.txt=\"");
    Serial.print("Requesting data to Device-1 . . .\""); 
    printnextion();

    Serial2.print("a1");
    listen();
    
    if (sdCardState == 1) {
      Serial.print("tProgress.txt=\"");
      Serial.print("Saving to SD card . . .\"");
      printnextion();
      payload = String(now.unixtime()) + String(";");
      payload += String(temp.temperature) + ";" + String(humidity.relative_humidity) + String(";;");
      payload += String(value1) + String(";") + String(value2) + String(";") + String(value3) + String(";");
      payload += String(temp1) + String(";") + String(temp2) + String(";") + String(temp3) + String(";");
      payload += String(hum1) + String(";") + String(hum2) + String(";") + String(temp3) + String(";");
      filePath = String("/") + String(now.year(), DEC) + String(now.month(), DEC) + String(now.day(), DEC) + ".csv";
      logSDCard();
      Serial.print("tProgress.txt=\"");
      Serial.print("Data saved to SD card . . .\"");
      printnextion();
    }

    Serial.print("tProgress.txt=\"");
    Serial.print("Push data to cloud . . .\"");
    printnextion();
    makeIFTTTRequest();

  }
}

void device2(){
  DateTime now = rtc.now();
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  if(millis() >= time_2 + slcDev2){
    time_2 += slcDev2;
    timestampDev2 = String(now.year(), DEC) + String("/") + String(now.month(), DEC) + String("/") + String(now.day(), DEC) + String(", ") + String(now.hour(), DEC) + String(":") + String(now.minute(), DEC) + String(":") + String(now.second(), DEC);
    timestampAvg = timestampDev2;
    printnextion();
    Serial.print("tProgress.txt=\"");
    Serial.print("Requesting data to Device-2 . . .\""); 
    printnextion();

    Serial2.print("a2");
    listen();

    if (sdCardState == 1) {
      Serial.print("tProgress.txt=\"");
      Serial.print("Saving to SD card . . .\"");
      printnextion();
      payload = String(now.unixtime()) + String(";");
      payload += String(temp.temperature) + ";" + String(humidity.relative_humidity) + String(";;");
      payload += String(value1) + String(";") + String(value2) + String(";") + String(value3) + String(";");
      payload += String(temp1) + String(";") + String(temp2) + String(";") + String(temp3) + String(";");
      payload += String(hum1) + String(";") + String(hum2) + String(";") + String(temp3) + String(";");
      filePath = String("/") + String(now.year(), DEC) + String(now.month(), DEC) + String(now.day(), DEC) + ".csv";
      logSDCard();
      Serial.print("tProgress.txt=\"");
      Serial.print("Data saved to SD card . . .\"");
      printnextion();
    }

    Serial.print("tProgress.txt=\"");
    Serial.print("Push data to cloud . . .\"");
    printnextion();
    makeIFTTTRequest();

  }
}

void device3(){
  DateTime now = rtc.now();
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  if(millis() >= time_3 + slcDev3){
    time_3 += slcDev3;
    timestampDev3 = String(now.year(), DEC) + String("/") + String(now.month(), DEC) + String("/") + String(now.day(), DEC) + String(", ") + String(now.hour(), DEC) + String(":") + String(now.minute(), DEC) + String(":") + String(now.second(), DEC);
    timestampAvg = timestampDev3;
    Serial.print("tProgress.txt=\"");
    Serial.print("Requesting data to Device-3 . . .\""); 
    printnextion();

    Serial2.print("a3");
    listen();

    if (sdCardState == 1) {
      Serial.print("tProgress.txt=\"");
      Serial.print("Saving to SD card . . .\"");
      printnextion();
      payload = String(now.unixtime()) + String(";");
      payload += String(temp.temperature) + ";" + String(humidity.relative_humidity) + String(";;");
      payload += String(value1) + String(";") + String(value2) + String(";") + String(value3) + String(";");
      payload += String(temp1) + String(";") + String(temp2) + String(";") + String(temp3) + String(";");
      payload += String(hum1) + String(";") + String(hum2) + String(";") + String(temp3) + String(";");
      filePath = String("/") + String(now.year(), DEC) + String(now.month(), DEC) + String(now.day(), DEC) + ".csv";
      logSDCard();
      Serial.print("tProgress.txt=\"");
      Serial.print("Data saved to SD card . . .\"");
      printnextion();
    }

    Serial.print("tProgress.txt=\"");
    Serial.print("Push data to cloud . . .\"");
    printnextion();
    makeIFTTTRequest();

  }
}

void powerCheck() {
  adc0Val = analogRead(adc0);
  adc1Val = analogRead(adc1);
  
  if (adc0Val < 2500) {
    relay0State = 0;
    mainPowerState = 1;
    digitalWrite(relay0, 0);
  } else {
    relay0State = 1;
    mainPowerState = 0;
    digitalWrite(relay0, 1);
  }

  if (adc1 < 3000) {
    batPowerState = 1;
  } else {
    batPowerState = 0;
  }
}

void checkPage() {
  Serial.println("cek halaman");
  DateTime now = rtc.now();
  // String timestampDev1 = String(now.year(), DEC) + String("/") + String(now.month(), DEC) + String("/") + String(now.day(), DEC) + String(", ") + String(now.hour(), DEC) + String(":") + String(now.minute(), DEC) + String(":") + String(now.second(), DEC);

  if (currentPage == 0) {
    printnextion();
    Serial.print("tLog0.txt=\"");
    Serial.print("System starting . . .\"");
    printnextion();
    Serial.print("tLog1.txt=\"");
    Serial.print("Serial communication established\"");
    printnextion();
    Serial.print("tLog2.txt=\"");
    Serial.print("Initializing SD card . . .\"");
    printnextion();
    if (sdCardState == 1) {
      Serial.print("tLog3.txt=\"");
      Serial.print("[OK] SD card mount success\"");
      printnextion();
      Serial.print("tLog4.txt=\"");
      uint8_t cardType = SD.cardType();
      if(cardType == CARD_NONE) {
        Serial.print("No SD card attached\"");
        return;
      }
      Serial.print("Initializing SD card type . . .\"");
      printnextion();
      Serial.print("tLog5.txt=\"");
      if(cardType == CARD_MMC){
        Serial.print("SD Card type is MMC\"");
      } else if(cardType == CARD_SD){
        Serial.print("SD Card type is SDSC\"");
      } else if(cardType == CARD_SDHC){
        Serial.print("SD Card type is SDHC\"");
      } else {
        Serial.print("SD Card type is unknown\"");
      }
      printnextion();
      uint64_t cardSize = SD.cardSize() / (1024 * 1024);
      Serial.print("tLog6.txt=\"");
      Serial.printf("SD card size: %lluMB\n\"", cardSize);
      printnextion();
    }

    Serial.print("tLog7.txt=\"");
    Serial.print("Establishing modbus RS485 communication (Device-1) . . .\"");
    printnextion();
    if (state1 == 1){
      Serial.print("tLog8.txt=\"");
      Serial.print("[OK] Slave-1 communication established\"");
      printnextion();
    } else {
      Serial.print("tLog8.txt=\"");
      Serial.print("[ERROR] Slave-1 communication failed\"");
      printnextion();
    }
    Serial.print("tLog9.txt=\"");
    Serial.print("Establishing modbus RS485 communication (Device-2) . . .\"");
    printnextion();
    if (state2 == 1){
      Serial.print("tLog10.txt=\"");
      Serial.print("[OK] Slave-2 communication established\"");
      printnextion();
    } else {
      Serial.print("tLog10.txt=\"");
      Serial.print("[ERROR] Slave-2 communication failed\"");
      printnextion();
    }
    Serial.print("tLog11.txt=\"");
    Serial.print("Establishing modbus RS485 communication (Device-3) . . .\"");
    printnextion();
    if (state3 == 1){
      Serial.print("tLog12.txt=\"");
      Serial.print("[OK] Slave-3 communication established\"");
    } else {
      Serial.print("tLog12.txt=\"");
      Serial.print("[ERROR] Slave-3 communication failed\"");
    }
    printnextion();

    Serial.print("tLog13.txt=\"");
    Serial.print("Establishing real time clock communication . . .\"");
    printnextion();
    Serial.print("tLog14.txt=\"");
    if (rtcState == 1) {
      Serial.print("[OK] Real time clock communication\"");
    } else {
      Serial.print("[ERROR] Real time clock communication\"");
    }
    printnextion();
    
    Serial.print("tLog15.txt=\"");
    Serial.print("Start scan Wi-Fi connection . . .\"");
    printnextion();

    if (wifiState == 0) {
      Serial.print("tLog16.txt=\"");
      Serial.print("[ERROR] No known network identified\"");
      printnextion();
      Serial.print("tLog17.txt=\"");
      Serial.print("[ERROR] No IP address\"");
      printnextion();
      Serial.print("tLog18.txt=\"");
      Serial.print("[ERROR] OTA not available\"");
      printnextion();
    } else {
      Serial.print("tLog16.txt=\"");
      if (ssidState == 1) {
        Serial.print("[OK] Wi-Fi connected, your SSID is Galaxy-Deny\"");
      } else if (ssidState == 2) {
        Serial.print("[OK] Wi-Fi connected, your SSID is INFORMATION CENTER\"");
      } else if (ssidState == 3) {
        Serial.print("[OK] Wi-Fi connected, your SSID is TP-Link_5FBA\"");
      } else if (ssidState == 4) {
        Serial.print("[OK] Wi-Fi connected, your SSID is smpitwasilah\"");
      } else {
        Serial.print("[ERROR] Wi-Fi not connected\"");
      }
      printnextion();
      Serial.print("tLog17.txt=\"");
      Serial.print("[OK] Wi-Fi connected, your IP address is ");
      Serial.print(WiFi.localIP());
      Serial.print("\"");
      printnextion();
      Serial.print("tLog18.txt=\"");
      Serial.print("[OK] OTA connected\"");
      printnextion();
    }
  } else if (currentPage == 1) {
    printnextion();
    if (wifiState == 0) {
      Serial.print("tIP.txt=\"");
      Serial.print("");
      Serial.print("\""); 
      printnextion();
      Serial.print("tWifi.txt=\"");
      Serial.print("No network connected\""); 
      printnextion();
    } else {
      Serial.print("tIP.txt=\"");
      Serial.print(WiFi.localIP());
      Serial.print("\""); 
      printnextion();
      if (ssidState == 1) {
        Serial.print("tWifi.txt=\"");
        Serial.print("Connected to Galaxy-Deny\""); 
        printnextion();
      } else if (ssidState == 2) {
        Serial.print("tWifi.txt=SSID = \"");
        Serial.print("Connected to INFORMATION CENTER\""); 
        printnextion();
      } else if (ssidState == 3) {
        Serial.print("tWifi.txt=\"");
        Serial.print("Connected to TP-Link_5FBA\""); 
        printnextion();
      } else if (ssidState == 4) {
        Serial.print("tWifi.txt=\"");
        Serial.print("Connected to smpitwasilah\""); 
        printnextion();
      }
    }
    Serial.print("tTimeAvg.txt=\"");
    Serial.print(timestampAvg); 
    Serial.print("\"");
    printnextion();
    Serial.print("tValAvg.txt=\"");
    Serial.print(averageValue); 
    Serial.print(" W/m^2\"");
    printnextion();
    Serial.print("tTimeWork.txt=\"");
    Serial.print(slcStart);
    Serial.print(" until ");
    Serial.print(slcEnd);
    Serial.print("\"");
    printnextion();
    if (state1 == 1) {
      Serial.print("pDev1.pic=13");
      printnextion();
      Serial.print("tValDev1.txt=\"");
      Serial.print(value1);
      Serial.print(" W/m^2\"");
      printnextion();
      Serial.print("tTimeDev1.txt=\"");
      Serial.print(timestampDev1);
      Serial.print("\"");
      printnextion();
      Serial.print("tSampling1.txt=\"");
      Serial.print(slcDev1);
      Serial.print(" ms\"");
      printnextion();
    }
    if (state2 == 1) {
      Serial.print("pDev2.pic=13");
      printnextion();
      Serial.print("tValDev2.txt=\"");
      Serial.print(value2);
      Serial.print(" W/m^2\"");
      printnextion();
      Serial.print("tTimeDev2.txt=\"");
      Serial.print(timestampDev2);
      Serial.print("\"");
      printnextion();
      Serial.print("tSampling2.txt=\"");
      Serial.print(slcDev2);
      Serial.print(" ms\"");
      printnextion();
    }
    if (state3 == 1) {
      Serial.print("pDev3.pic=13");
      printnextion();
      Serial.print("tValDev3.txt=\"");
      Serial.print(value3);
      Serial.print(" W/m^2\"");
      printnextion();
      Serial.print("tTimeDev3.txt=\"");
      Serial.print(timestampDev3);
      Serial.print("\"");
      printnextion();
      Serial.print("tSampling3.txt=\"");
      Serial.print(slcDev3);
      Serial.print(" ms\"");
      printnextion();
    }
  } else if (currentPage == 2) {
    printnextion();
    Serial.print("tPre1Dev1.txt=\"");
    Serial.print(preset1);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre2Dev1.txt=\"");
    Serial.print(preset2);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre3Dev1.txt=\"");
    Serial.print(preset3);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre4Dev1.txt=\"");
    Serial.print(preset4);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre5Dev1.txt=\"");
    Serial.print(preset5);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre6Dev1.txt=\"");
    Serial.print(preset6);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre7Dev1.txt=\"");
    Serial.print(preset7);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre8Dev1.txt=\"");
    Serial.print(preset8);
    Serial.print(" ms\"");
    printnextion();
  } else if (currentPage == 3) {
    printnextion();
    Serial.print("tPre1Dev2.txt=\"");
    Serial.print(preset1);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre2Dev2.txt=\"");
    Serial.print(preset2);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre3Dev2.txt=\"");
    Serial.print(preset3);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre4Dev2.txt=\"");
    Serial.print(preset4);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre5Dev2.txt=\"");
    Serial.print(preset5);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre6Dev2.txt=\"");
    Serial.print(preset6);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre7Dev2.txt=\"");
    Serial.print(preset7);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre8Dev2.txt=\"");
    Serial.print(preset8);
    Serial.print(" ms\"");
    printnextion();
  } else if (currentPage == 4) {
    printnextion();
    Serial.print("tPre1Dev3.txt=\"");
    Serial.print(preset1);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre2Dev3.txt=\"");
    Serial.print(preset2);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre3Dev3.txt=\"");
    Serial.print(preset3);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre4Dev3.txt=\"");
    Serial.print(preset4);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre5Dev3.txt=\"");
    Serial.print(preset5);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre6Dev3.txt=\"");
    Serial.print(preset6);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre7Dev3.txt=\"");
    Serial.print(preset7);
    Serial.print(" ms\"");
    printnextion();
    Serial.print("tPre8Dev3.txt=\"");
    Serial.print(preset8);
    Serial.print(" ms\"");
    printnextion();
  } else if (currentPage == 13) {
    printnextion();
    Serial.print("tStart1.txt=\"");
    Serial.print(start1);
    Serial.print("\"");
    printnextion();
    Serial.print("tStart2.txt=\"");
    Serial.print(start2);
    Serial.print("\"");
    printnextion();
    Serial.print("tStart3.txt=\"");
    Serial.print(start3);
    Serial.print("\"");
    printnextion();
    Serial.print("tStart4.txt=\"");
    Serial.print(start4);
    Serial.print("\"");
    printnextion();
    Serial.print("tStart5.txt=\"");
    Serial.print(start5);
    Serial.print("\"");
    printnextion();
    Serial.print("tEnd1.txt=\"");
    Serial.print(end1);
    Serial.print("\"");
    printnextion();
    Serial.print("tEnd2.txt=\"");
    Serial.print(end2);
    Serial.print("\"");
    printnextion();
    Serial.print("tEnd3.txt=\"");
    Serial.print(end3);
    Serial.print("\"");
    printnextion();
    Serial.print("tEnd4.txt=\"");
    Serial.print(end4);
    Serial.print("\"");
    printnextion();
    Serial.print("tEnd5.txt=\"");
    Serial.print(end5);
    Serial.print("\"");
    printnextion();
  }
}

void setup() { 
  Serial.begin(9600);
  Serial2.setTimeout(250);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Wire.begin();
  nexInit();
  pinMode(Enable, OUTPUT);

  delay(2000);
  Serial.println("sampai sini");
  
  printnextion();
  Serial.print("tLog0.txt=\"");
  Serial.print("System starting . . .\"");
  printnextion();

  delay(5000);
  checkPeripheral();
  initWifi();

  // Register the pop event callback function of the components
  page0.attachPop(page0PopCallback, &page0);
  tLog0.attachPop(tLog0PopCallback, &tLog0);
  tLog1.attachPop(tLog1PopCallback, &tLog1);
  tLog2.attachPop(tLog2PopCallback, &tLog2);
  tLog3.attachPop(tLog3PopCallback, &tLog3);
  tLog4.attachPop(tLog4PopCallback, &tLog4);
  tLog5.attachPop(tLog5PopCallback, &tLog5);
  tLog6.attachPop(tLog6PopCallback, &tLog6);
  tLog7.attachPop(tLog7PopCallback, &tLog7);
  tLog8.attachPop(tLog8PopCallback, &tLog8);
  tLog9.attachPop(tLog9PopCallback, &tLog9);
  tLog10.attachPop(tLog10PopCallback, &tLog10);
  tLog11.attachPop(tLog11PopCallback, &tLog11);
  tLog12.attachPop(tLog12PopCallback, &tLog12);
  tLog13.attachPop(tLog13PopCallback, &tLog13);
  tLog14.attachPop(tLog14PopCallback, &tLog14);
  tLog15.attachPop(tLog15PopCallback, &tLog15);
  tLog16.attachPop(tLog16PopCallback, &tLog16);
  tLog17.attachPop(tLog17PopCallback, &tLog17);
  tLog18.attachPop(tLog18PopCallback, &tLog18);
  tLog19.attachPop(tLog19PopCallback, &tLog19);
  tLog20.attachPop(tLog20PopCallback, &tLog20);

  pDev1.attachPop(pDev1PopCallback, &pDev1);
  pDev2.attachPop(pDev2PopCallback, &pDev2);
  pDev3.attachPop(pDev3PopCallback, &pDev3);
  bDebug.attachPop(bDebugPopCallback, &bDebug);
  bKey.attachPop(bKeyPopCallback, &bKey);
  bKeyHome.attachPop(bKeyHomePopCallback, &bKeyHome);
  bKeyTele.attachPop(bKeyTelePopCallback, &bKeyTele);
  bKeyTeleHome.attachPop(bKeyTeleHomePopCallback, &bKeyTeleHome);
  bKeyGraph.attachPop(bKeyGraphPopCallback, &bKeyGraph);
  bKeyGraphHome.attachPop(bKeyGraphHomePopCallback, &bKeyGraphHome);
  bKeySheet.attachPop(bKeySheetPopCallback, &bKeySheet);
  bKeySheetHome.attachPop(bKeySheetHomePopCallback, &bKeySheetHome);
  bInfo.attachPop(bInfoPopCallback, &bInfo);
  bInfoHome.attachPop(bInfoHomePopCallback, &bInfoHome);
  bInfoWA.attachPop(bInfoWAPopCallback, &bInfoWA);
  bInfoWAHome.attachPop(bInfoWAHomePopCallback, &bInfoWAHome);
  bInfoTele.attachPop(bInfoTelePopCallback, &bInfoTele);
  bInfoTeleHome.attachPop(bInfoTeleHomePopCallback, &bInfoTeleHome);
  bInfoMail.attachPop(bInfoMailPopCallback, &bInfoMail);
  bInfoMailHome.attachPop(bInfoMailHomePopCallback, &bInfoMailHome);
  bTimeWork.attachPop(bTimeWorkPopCallback, &bTimeWork);

  tPre1Dev1.attachPop(tPre1Dev1PopCallback, &tPre1Dev1);
  tPre2Dev1.attachPop(tPre2Dev1PopCallback, &tPre2Dev1);
  tPre3Dev1.attachPop(tPre3Dev1PopCallback, &tPre3Dev1);
  tPre4Dev1.attachPop(tPre4Dev1PopCallback, &tPre4Dev1);
  tPre5Dev1.attachPop(tPre5Dev1PopCallback, &tPre5Dev1);
  tPre6Dev1.attachPop(tPre6Dev1PopCallback, &tPre6Dev1);
  tPre7Dev1.attachPop(tPre7Dev1PopCallback, &tPre7Dev1);
  tPre8Dev1.attachPop(tPre8Dev1PopCallback, &tPre8Dev1);

  tPre1Dev2.attachPop(tPre1Dev2PopCallback, &tPre1Dev2);
  tPre2Dev2.attachPop(tPre2Dev2PopCallback, &tPre2Dev2);
  tPre3Dev2.attachPop(tPre3Dev2PopCallback, &tPre3Dev2);
  tPre4Dev2.attachPop(tPre4Dev2PopCallback, &tPre4Dev2);
  tPre5Dev2.attachPop(tPre5Dev2PopCallback, &tPre5Dev2);
  tPre6Dev2.attachPop(tPre6Dev2PopCallback, &tPre6Dev2);
  tPre7Dev2.attachPop(tPre7Dev2PopCallback, &tPre7Dev2);
  tPre8Dev2.attachPop(tPre8Dev2PopCallback, &tPre8Dev2);
  
  tPre1Dev3.attachPop(tPre1Dev3PopCallback, &tPre1Dev3);
  tPre2Dev3.attachPop(tPre2Dev3PopCallback, &tPre2Dev3);
  tPre3Dev3.attachPop(tPre3Dev3PopCallback, &tPre3Dev3);
  tPre4Dev3.attachPop(tPre4Dev3PopCallback, &tPre4Dev3);
  tPre5Dev3.attachPop(tPre5Dev3PopCallback, &tPre5Dev3);
  tPre6Dev3.attachPop(tPre6Dev3PopCallback, &tPre6Dev3);
  tPre7Dev3.attachPop(tPre7Dev3PopCallback, &tPre7Dev3);
  tPre8Dev3.attachPop(tPre8Dev3PopCallback, &tPre8Dev3);

  tStart1.attachPop(tStart1PopCallback, &tStart1);
  tStart2.attachPop(tStart2PopCallback, &tStart2);
  tStart3.attachPop(tStart3PopCallback, &tStart3);
  tStart4.attachPop(tStart4PopCallback, &tStart4);
  tStart5.attachPop(tStart5PopCallback, &tStart5);
  tEnd1.attachPop(tEnd1PopCallback, &tEnd1);
  tEnd2.attachPop(tEnd2PopCallback, &tEnd2);
  tEnd3.attachPop(tEnd3PopCallback, &tEnd3);
  tEnd4.attachPop(tEnd4PopCallback, &tEnd4);
  tEnd5.attachPop(tEnd5PopCallback, &tEnd5);
/////////////////////////////////////////////////////////////////////////////////
  // Register the pop event callback function of the components
  page0.attachPush(page0PushCallback, &page0);
  tLog0.attachPush(tLog0PushCallback, &tLog0);
  tLog1.attachPush(tLog1PushCallback, &tLog1);
  tLog2.attachPush(tLog2PushCallback, &tLog2);
  tLog3.attachPush(tLog3PushCallback, &tLog3);
  tLog4.attachPush(tLog4PushCallback, &tLog4);
  tLog5.attachPush(tLog5PushCallback, &tLog5);
  tLog6.attachPush(tLog6PushCallback, &tLog6);
  tLog7.attachPush(tLog7PushCallback, &tLog7);
  tLog8.attachPush(tLog8PushCallback, &tLog8);
  tLog9.attachPush(tLog9PushCallback, &tLog9);
  tLog10.attachPush(tLog10PushCallback, &tLog10);
  tLog11.attachPush(tLog11PushCallback, &tLog11);
  tLog12.attachPush(tLog12PushCallback, &tLog12);
  tLog13.attachPush(tLog13PushCallback, &tLog13);
  tLog14.attachPush(tLog14PushCallback, &tLog14);
  tLog15.attachPush(tLog15PushCallback, &tLog15);
  tLog16.attachPush(tLog16PushCallback, &tLog16);
  tLog17.attachPush(tLog17PushCallback, &tLog17);
  tLog18.attachPush(tLog18PushCallback, &tLog18);
  tLog19.attachPush(tLog19PushCallback, &tLog19);
  tLog20.attachPush(tLog20PushCallback, &tLog20);

  pDev1.attachPush(pDev1PushCallback, &pDev1);
  pDev2.attachPush(pDev2PushCallback, &pDev2);
  pDev3.attachPush(pDev3PushCallback, &pDev3);
  bDebug.attachPush(bDebugPushCallback, &bDebug);
  bKey.attachPush(bKeyPushCallback, &bKey);
  bKeyHome.attachPush(bKeyHomePushCallback, &bKeyHome);
  bKeyTele.attachPush(bKeyTelePushCallback, &bKeyTele);
  bKeyTeleHome.attachPush(bKeyTeleHomePushCallback, &bKeyTeleHome);
  bKeyGraph.attachPush(bKeyGraphPushCallback, &bKeyGraph);
  bKeyGraphHome.attachPush(bKeyGraphHomePushCallback, &bKeyGraphHome);
  bKeySheet.attachPush(bKeySheetPushCallback, &bKeySheet);
  bKeySheetHome.attachPush(bKeySheetHomePushCallback, &bKeySheetHome);
  bInfo.attachPush(bInfoPushCallback, &bInfo);
  bInfoHome.attachPush(bInfoHomePushCallback, &bInfoHome);
  bInfoWA.attachPush(bInfoWAPushCallback, &bInfoWA);
  bInfoWAHome.attachPush(bInfoWAHomePushCallback, &bInfoWAHome);
  bInfoTele.attachPush(bInfoTelePushCallback, &bInfoTele);
  bInfoTeleHome.attachPush(bInfoTeleHomePushCallback, &bInfoTeleHome);
  bInfoMail.attachPush(bInfoMailPushCallback, &bInfoMail);
  bInfoMailHome.attachPush(bInfoMailHomePushCallback, &bInfoMailHome);
  bTimeWork.attachPush(bTimeWorkPushCallback, &bTimeWork);

  tPre1Dev1.attachPush(tPre1Dev1PushCallback, &tPre1Dev1);
  tPre2Dev1.attachPush(tPre2Dev1PushCallback, &tPre2Dev1);
  tPre3Dev1.attachPush(tPre3Dev1PushCallback, &tPre3Dev1);
  tPre4Dev1.attachPush(tPre4Dev1PushCallback, &tPre4Dev1);
  tPre5Dev1.attachPush(tPre5Dev1PushCallback, &tPre5Dev1);
  tPre6Dev1.attachPush(tPre6Dev1PushCallback, &tPre6Dev1);
  tPre7Dev1.attachPush(tPre7Dev1PushCallback, &tPre7Dev1);
  tPre8Dev1.attachPush(tPre8Dev1PushCallback, &tPre8Dev1);

  tPre1Dev2.attachPush(tPre1Dev2PushCallback, &tPre1Dev2);
  tPre2Dev2.attachPush(tPre2Dev2PushCallback, &tPre2Dev2);
  tPre3Dev2.attachPush(tPre3Dev2PushCallback, &tPre3Dev2);
  tPre4Dev2.attachPush(tPre4Dev2PushCallback, &tPre4Dev2);
  tPre5Dev2.attachPush(tPre5Dev2PushCallback, &tPre5Dev2);
  tPre6Dev2.attachPush(tPre6Dev2PushCallback, &tPre6Dev2);
  tPre7Dev2.attachPush(tPre7Dev2PushCallback, &tPre7Dev2);
  tPre8Dev2.attachPush(tPre8Dev2PushCallback, &tPre8Dev2);
  
  tPre1Dev3.attachPush(tPre1Dev3PushCallback, &tPre1Dev3);
  tPre2Dev3.attachPush(tPre2Dev3PushCallback, &tPre2Dev3);
  tPre3Dev3.attachPush(tPre3Dev3PushCallback, &tPre3Dev3);
  tPre4Dev3.attachPush(tPre4Dev3PushCallback, &tPre4Dev3);
  tPre5Dev3.attachPush(tPre5Dev3PushCallback, &tPre5Dev3);
  tPre6Dev3.attachPush(tPre6Dev3PushCallback, &tPre6Dev3);
  tPre7Dev3.attachPush(tPre7Dev3PushCallback, &tPre7Dev3);
  tPre8Dev3.attachPush(tPre8Dev3PushCallback, &tPre8Dev3);

  tStart1.attachPush(tStart1PushCallback, &tStart1);
  tStart2.attachPush(tStart2PushCallback, &tStart2);
  tStart3.attachPush(tStart3PushCallback, &tStart3);
  tStart4.attachPush(tStart4PushCallback, &tStart4);
  tStart5.attachPush(tStart5PushCallback, &tStart5);
  tEnd1.attachPush(tEnd1PushCallback, &tEnd1);
  tEnd2.attachPush(tEnd2PushCallback, &tEnd2);
  tEnd3.attachPush(tEnd3PushCallback, &tEnd3);
  tEnd4.attachPush(tEnd4PushCallback, &tEnd4);
  tEnd5.attachPush(tEnd5PushCallback, &tEnd5);
/////////////////////////////////////////////////////////////////////////////////
  delay(3000);
  printnextion();
  Serial.print("page 1");
  printnextion();
  currentPage = 1;

  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
}

void loop() {
  DateTime now = rtc.now();
  String timeNow = String(daysOfTheWeek[now.dayOfTheWeek()]) + String(" - ") + String(now.day(), DEC) + String("/") + String(now.month(), DEC) + String("/") + String(now.year(), DEC) + String(" - ") + String(now.hour(), DEC) + String(":") + String(now.minute(), DEC) + String(":") + String(now.second(), DEC);

  if (wifiState == 1) {
    if (millis() > lastTimeBotRan + botRequestDelay)  {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

      while(numNewMessages) {
        Serial2.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
      lastTimeBotRan = millis();
    }
  }

  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }

  if (time_1 == time_2) {
    time_1 += 2000;
  } else if (time_1 == time_3) {
    time_3 += 2000;
  } else if (time_2 == time_3) {
    time_2 += 2000;
  }

  if ((now.hour() > (slcStart-1)) && (now.hour() < (slcEnd-1))) {
    stateMaster = 1;
  } else {
    stateMaster = 0;
  }

  if (currentPage == 1) {
    // if (stateMaster == 1 && batPowerState == 1 && state1 == 1 && state2 == 1 && state3 == 1) {
    // if (state1 == 1 && state2 == 1 && state3 == 1) {
    if (stateMaster == 1 && state1 == 1 && state2 == 1 && state3 == 1) {
      device1();
      device2();
      device3();
      averageValue = (value1+value2+value3)/3;
    // } else if (stateMaster == 1 && batPowerState == 1 && state1 == 1 && state2 == 1 && state3 == 0) {
    // } else if (state1 == 1 && state2 == 1 && state3 == 0) {
    } else if (stateMaster == 1 && state1 == 1 && state2 == 1 && state3 == 0) {
      device1();
      device2();
      averageValue = (value1+value2)/2;
      checkDevice3();
    // } else if (stateMaster == 1 && batPowerState == 1 && state1 == 1 && state2 == 0 && state3 == 1) {
    // } else if (state1 == 1 && state2 == 0 && state3 == 1) {
    } else if (stateMaster == 1 && state1 == 1 && state2 == 0 && state3 == 1) {
      device1();
      device3();
      averageValue = (value1+value3)/2;
      // checkDevice2;
    // } else if (stateMaster == 1 && batPowerState == 1 && state1 == 0 && state2 == 1 && state3 == 1) {
    // } else if (state1 == 0 && state2 == 1 && state3 == 1) {
    } else if (stateMaster == 1 && state1 == 0 && state2 == 1 && state3 == 1) {
      device2();
      device3();
      averageValue = (value2+value3)/2;
      // checkDevice1();
    // } else if (stateMaster == 1 && batPowerState == 1 && state1 == 1 && state2 == 0 && state3 == 0) {
    // } else if (state1 == 1 && state2 == 0 && state3 == 0) {
    } else if (stateMaster == 1 && state1 == 1 && state2 == 0 && state3 == 0) {
      device1();
      averageValue = value1;
      // checkDevice2();
      // checkDevice3();
    // } else if (stateMaster == 1 && batPowerState == 1 && state1 == 0 && state2 == 1 && state3 == 0) {
    // } else if (state1 == 0 && state2 == 1 && state3 == 0) {
    } else if (stateMaster == 1 && state1 == 0 && state2 == 1 && state3 == 0) {
      device2();
      averageValue = value2;
      // checkDevice1();
      // checkDevice3();
    // } else if (stateMaster == 1 && batPowerState == 1 && state1 == 0 && state2 == 0 && state3 == 1) {
    // } else if (state1 == 0 && state2 == 0 && state3 == 1) {
    } else if (stateMaster == 1 && state1 == 0 && state2 == 0 && state3 == 1) {
      device3();
      averageValue = value3;
      // checkDevice1();
      // checkDevice2();
    } else if (stateMaster == 1 && state1 == 0 && state2 == 0 && state3 == 0) {
      printnextion();
      Serial.print("tProgress.txt=\"");
      Serial.print("No device connected\""); 
      printnextion();
      // checkDevice1();
      // checkDevice2();
      // checkDevice3();
    } else {
      printnextion();
      Serial.print("tProgress.txt=\"");
      Serial.print("Day night, time to sleep . . . \""); 
    }
  }

  if (stateMaster == 1) {
    printnextion();
    Serial.print("dim=70");
    printnextion();
  } else {
    printnextion();
    Serial.print("dim=10");
    printnextion();
  }

  printnextion();
  Serial.print("tTimeNow.txt=\"");
  Serial.print(timeNow); 
  Serial.print("\""); 
  printnextion();
  
  Serial.print("currentPage= ");
  Serial.println(currentPage);

  checkPage();
  nexLoop(nex_listen_list);
}