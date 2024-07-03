#include "RTClib.h"

extern bool rtcState;

String update_rtc_unix();
String update_rtc_year();
String update_rtc_month();
String update_rtc_day();
String update_rtc_hour();
String update_rtc_minute();
String update_rtc_second();
String update_rtc_dayOfTheWeek();

void init_rtc();