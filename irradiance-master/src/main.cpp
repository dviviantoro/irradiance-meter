#include <Arduino.h>
#include <common.h>
#include <Wire.h>
#include "Nextion.h"    

#include <general.h>
#include <wireless_com.h>
#include <rtc-ao.h>
#include <sd-ao.h>

unsigned long previousMillis_routine = 0;

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

void register_nextion_callback()
{
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

String time_now()
{
    String timeNow = update_rtc_dayOfTheWeek() + String(" - ");
    timeNow += update_rtc_day() + String("/");
    timeNow += update_rtc_month() + String("/");
    timeNow += update_rtc_year() + String(" - ");
    timeNow += update_rtc_hour() + ":";
    timeNow += update_rtc_minute() + ":";
    timeNow += update_rtc_second();
    return timeNow;
}

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    
    nexInit();
    pinMode(Enable, OUTPUT);

    nextion_separator();
    Serial.print("page 0");
    nextion_separator();
    delay(2000);

    Serial.print("tLog0.txt=\"");
    Serial.print("System starting . . .\"");
    nextion_separator();

    delay(5000);
    init_peripheral();
    initWifi();

    register_nextion_callback();
  
    delay(3000);
    nextion_separator();
    Serial.print("page 1");
    nextion_separator();
    currentPage = 1;
}

void loop()
{
    ElegantOTA.loop();
    nexLoop(nex_listen_list);
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis_routine >= interval)
    {
        check_page();
        
        if (stateMaster == 1) {
            nextion_separator();
            Serial.print("dim=70");
            nextion_separator();
        }
        else
        {
            nextion_separator();
            Serial.print("dim=10");
            nextion_separator();
        }

        nextion_separator();
        Serial.print("tTimeNow.txt=\"");
        Serial.print(time_now()); 
        Serial.print("\""); 
        nextion_separator();
    
        Serial.print("currentPage= ");
        Serial.println(currentPage);

        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(millis());
            Serial.println("Reconnecting to WiFi...");
            WiFi.disconnect();
            WiFi.reconnect();
        }
        previousMillis_routine = currentMillis;
    }

    if (time_1 == time_2)
    {
        time_1 += 2000;
    }
    else if (time_1 == time_3)
    {
        time_3 += 2000;
    }
    else if (time_2 == time_3)
    {
        time_2 += 2000;
    }

    (update_rtc_hour().toInt() > (slcStart-1)) && (update_rtc_hour().toInt() < (slcEnd-1)) ? stateMaster = 1 : stateMaster = 0;
    
    if (currentPage == 1)
    {
        collect_data();
    }

}