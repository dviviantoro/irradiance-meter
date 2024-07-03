#include <rtc-ao.h>
#include <general.h>

RTC_DS1307 rtc;
bool rtcState = false;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void init_rtc()
{
    Serial.print("tLog13.txt=\"");
    Serial.print("Establishing real time clock communication . . .\"");
    nextion_separator();
    rtcState = 1;

    Serial.print("tLog14.txt=\"");
    if (! rtc.begin())
    {
        Serial.print("[ERROR] Real time clock communication\"");
        nextion_separator();
    }
    Serial.print("[OK] Real time clock communication\"");
    nextion_separator();

    if (! rtc.isrunning())
    {
        // Serial2.println("RTC is NOT running, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2023, 3, 16, 12, 59, 0));
    }
}

String update_rtc_unix()
{
    DateTime now = rtc.now();
    String unix = String(now.unixtime());
    return unix;
}

String update_rtc_year()
{
    DateTime now = rtc.now();
    String year = String(now.year(), DEC);
    return year;
}

String update_rtc_month()
{
    DateTime now = rtc.now();
    String month = String(now.month(), DEC);
    return month;
}

String update_rtc_day()
{
    DateTime now = rtc.now();
    String day = String(now.day(), DEC);
    return day;
}

String update_rtc_hour()
{
    DateTime now = rtc.now();
    String hour = String(now.hour(), DEC);
    return hour;
}

String update_rtc_minute()
{
    DateTime now = rtc.now();
    String minute = String(now.minute(), DEC);
    return minute;
}

String update_rtc_second()
{
    DateTime now = rtc.now();
    String second = String(now.second(), DEC);
    return second;
}

String update_rtc_dayOfTheWeek()
{
    DateTime now = rtc.now();
    String day = String(daysOfTheWeek[now.dayOfTheWeek()]);
    return day;
}