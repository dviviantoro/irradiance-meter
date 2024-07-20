#include <general.h>
#include <modbus-ao.h>
#include <rtc-ao.h>
#include <sensor-ao.h>
#include <sd-ao.h>
#include <wireless_com.h>

unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long time_3 = 0;
unsigned long time_4 = 0;

int preset1 = 5000;
int preset2 = 20000;
int preset3 = 30000;
int preset4 = 40000;
int preset5 = 50000;
int preset6 = 60000;
int preset7 = 70000;
int preset8 = 80000;

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

int slcDev1 = preset6;
int slcDev2 = preset7;
int slcDev3 = preset8;

int slcStart = start3;
int slcEnd = end3;

int currentPage = 0;
String timestampDev1, timestampDev2, timestampDev3, timestampAvg;
int averageValue = 0;

bool state1 = false, state2 = false, state3 = false, stateMaster = false;

void nextion_separator()
{
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
}

void update_page_0()
{
    nextion_separator();
    
    Serial.print("tLog0.txt=\"");
    Serial.print("System starting . . .\"");
    nextion_separator();

    Serial.print("tLog1.txt=\"");
    Serial.print("Serial communication established\"");
    nextion_separator();
    
    Serial.print("tLog2.txt=\"");
    Serial.print("Initializing SD card . . .\"");
    nextion_separator();

    if (sdCardState == 1)
    {
        Serial.print("tLog3.txt=\"");
        Serial.print("[OK] SD card mount success\"");
        nextion_separator();
        
        properties_sd();
    }

    Serial.print("tLog7.txt=\"");
    Serial.print("Establishing modbus RS485 communication (Device-1) . . .\"");
    nextion_separator();
    if (state1 == 1)
    {
        Serial.print("tLog8.txt=\"");
        Serial.print("[OK] Slave-1 communication established\"");
    }
    else
    {
        Serial.print("tLog8.txt=\"");
        Serial.print("[ERROR] Slave-1 communication failed\"");
    }
    nextion_separator();

    Serial.print("tLog9.txt=\"");
    Serial.print("Establishing modbus RS485 communication (Device-2) . . .\"");
    nextion_separator();
    if (state2 == 1)
    {
        Serial.print("tLog10.txt=\"");
        Serial.print("[OK] Slave-2 communication established\"");
    }
    else
    {
        Serial.print("tLog10.txt=\"");
        Serial.print("[ERROR] Slave-2 communication failed\"");
    }
    nextion_separator();

    Serial.print("tLog11.txt=\"");
    Serial.print("Establishing modbus RS485 communication (Device-3) . . .\"");
    nextion_separator();
    if (state3 == 1)
    {
        Serial.print("tLog12.txt=\"");
        Serial.print("[OK] Slave-3 communication established\"");
    }
    else
    {
        Serial.print("tLog12.txt=\"");
        Serial.print("[ERROR] Slave-3 communication failed\"");
    }
    nextion_separator();

    Serial.print("tLog13.txt=\"");
    Serial.print("Establishing real time clock communication . . .\"");
    nextion_separator();

    Serial.print("tLog14.txt=\"");
    rtcState == 1 ? Serial.print("[OK] Real time clock communication\"") : Serial.print("[ERROR] Real time clock communication\"");
    nextion_separator();
    
    Serial.print("tLog15.txt=\"");
    Serial.print("Start scan Wi-Fi connection . . .\"");
    nextion_separator();

    if (wifiState == 0)
    {
        Serial.print("tLog16.txt=\"");
        Serial.print("[ERROR] No known network identified\"");
        nextion_separator();
        Serial.print("tLog17.txt=\"");
        Serial.print("[ERROR] No IP address\"");
        nextion_separator();
        Serial.print("tLog18.txt=\"");
        Serial.print("[ERROR] OTA not available\"");
        nextion_separator();
    }
    else
    {
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
        nextion_separator();
        Serial.print("tLog17.txt=\"");
        Serial.print("[OK] Wi-Fi connected, your IP address is ");
        Serial.print(WiFi.localIP());
        Serial.print("\"");
        nextion_separator();
        Serial.print("tLog18.txt=\"");
        Serial.print("[OK] OTA connected\"");
        nextion_separator();
    }
}

void update_page_1()
{
    nextion_separator();
    if (wifiState == 0)
    {
        Serial.print("tIP.txt=\"");
        Serial.print("");
        Serial.print("\""); 
        nextion_separator();
        Serial.print("tWifi.txt=\"");
        Serial.print("No network connected\""); 
        nextion_separator();
    }
    else
    {
        Serial.print("tIP.txt=\"");
        Serial.print(WiFi.localIP());
        Serial.print("\""); 
        nextion_separator();
        if (ssidState == 1)
        {
            Serial.print("tWifi.txt=\"");
            Serial.print("Connected to Galaxy-Deny\""); 
            nextion_separator();
        }
        else if (ssidState == 2)
        {
            Serial.print("tWifi.txt=SSID = \"");
            Serial.print("Connected to INFORMATION CENTER\""); 
            nextion_separator();
        }
        else if (ssidState == 3)
        {
            Serial.print("tWifi.txt=\"");
            Serial.print("Connected to TP-Link_5FBA\""); 
            nextion_separator();
        }
        else if (ssidState == 4)
        {
            Serial.print("tWifi.txt=\"");
            Serial.print("Connected to smpitwasilah\""); 
            nextion_separator();
        }
    }
    Serial.print("tTimeAvg.txt=\"");
    Serial.print(timestampAvg); 
    Serial.print("\"");
    nextion_separator();
    Serial.print("tValAvg.txt=\"");
    Serial.print(averageValue); 
    Serial.print(" W/m^2\"");
    nextion_separator();
    Serial.print("tTimeWork.txt=\"");
    Serial.print(slcStart);
    Serial.print(" until ");
    Serial.print(slcEnd);
    Serial.print("\"");
    nextion_separator();

    if (state1 == 1)
    {
        Serial.print("pDev1.pic=13");
        nextion_separator();
        Serial.print("tValDev1.txt=\"");
        Serial.print(value1);
        Serial.print(" W/m^2\"");
        nextion_separator();
        Serial.print("tTimeDev1.txt=\"");
        Serial.print(timestampDev1);
        Serial.print("\"");
        nextion_separator();
        Serial.print("tSampling1.txt=\"");
        Serial.print(slcDev1);
        Serial.print(" ms\"");
        nextion_separator();
    }
    
    if (state2 == 1)
    {
        Serial.print("pDev2.pic=13");
        nextion_separator();
        Serial.print("tValDev2.txt=\"");
        Serial.print(value2);
        Serial.print(" W/m^2\"");
        nextion_separator();
        Serial.print("tTimeDev2.txt=\"");
        Serial.print(timestampDev2);
        Serial.print("\"");
        nextion_separator();
        Serial.print("tSampling2.txt=\"");
        Serial.print(slcDev2);
        Serial.print(" ms\"");
        nextion_separator();
    }

    if (state3 == 1)
    {
        Serial.print("pDev3.pic=13");
        nextion_separator();
        Serial.print("tValDev3.txt=\"");
        Serial.print(value3);
        Serial.print(" W/m^2\"");
        nextion_separator();
        Serial.print("tTimeDev3.txt=\"");
        Serial.print(timestampDev3);
        Serial.print("\"");
        nextion_separator();
        Serial.print("tSampling3.txt=\"");
        Serial.print(slcDev3);
        Serial.print(" ms\"");
        nextion_separator();
    }
}

void update_page_2()
{
    nextion_separator();
    Serial.print("tPre1Dev1.txt=\"");
    Serial.print(preset1);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre2Dev1.txt=\"");
    Serial.print(preset2);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre3Dev1.txt=\"");
    Serial.print(preset3);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre4Dev1.txt=\"");
    Serial.print(preset4);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre5Dev1.txt=\"");
    Serial.print(preset5);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre6Dev1.txt=\"");
    Serial.print(preset6);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre7Dev1.txt=\"");
    Serial.print(preset7);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre8Dev1.txt=\"");
    Serial.print(preset8);
    Serial.print(" ms\"");
    nextion_separator();
}

void update_page_3()
{
    nextion_separator();
    Serial.print("tPre1Dev2.txt=\"");
    Serial.print(preset1);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre2Dev2.txt=\"");
    Serial.print(preset2);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre3Dev2.txt=\"");
    Serial.print(preset3);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre4Dev2.txt=\"");
    Serial.print(preset4);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre5Dev2.txt=\"");
    Serial.print(preset5);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre6Dev2.txt=\"");
    Serial.print(preset6);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre7Dev2.txt=\"");
    Serial.print(preset7);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre8Dev2.txt=\"");
    Serial.print(preset8);
    Serial.print(" ms\"");
    nextion_separator();
}

void update_page_4()
{
    nextion_separator();
    Serial.print("tPre1Dev3.txt=\"");
    Serial.print(preset1);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre2Dev3.txt=\"");
    Serial.print(preset2);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre3Dev3.txt=\"");
    Serial.print(preset3);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre4Dev3.txt=\"");
    Serial.print(preset4);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre5Dev3.txt=\"");
    Serial.print(preset5);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre6Dev3.txt=\"");
    Serial.print(preset6);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre7Dev3.txt=\"");
    Serial.print(preset7);
    Serial.print(" ms\"");
    nextion_separator();
    Serial.print("tPre8Dev3.txt=\"");
    Serial.print(preset8);
    Serial.print(" ms\"");
    nextion_separator();
}

void update_page_13()
{
    nextion_separator();
    Serial.print("tStart1.txt=\"");
    Serial.print(start1);
    Serial.print("\"");
    nextion_separator();
    Serial.print("tStart2.txt=\"");
    Serial.print(start2);
    Serial.print("\"");
    nextion_separator();
    Serial.print("tStart3.txt=\"");
    Serial.print(start3);
    Serial.print("\"");
    nextion_separator();
    Serial.print("tStart4.txt=\"");
    Serial.print(start4);
    Serial.print("\"");
    nextion_separator();
    Serial.print("tStart5.txt=\"");
    Serial.print(start5);
    Serial.print("\"");
    nextion_separator();
    Serial.print("tEnd1.txt=\"");
    Serial.print(end1);
    Serial.print("\"");
    nextion_separator();
    Serial.print("tEnd2.txt=\"");
    Serial.print(end2);
    Serial.print("\"");
    nextion_separator();
    Serial.print("tEnd3.txt=\"");
    Serial.print(end3);
    Serial.print("\"");
    nextion_separator();
    Serial.print("tEnd4.txt=\"");
    Serial.print(end4);
    Serial.print("\"");
    nextion_separator();
    Serial.print("tEnd5.txt=\"");
    Serial.print(end5);
    Serial.print("\"");
    nextion_separator();
}

void check_page() {
    Serial.println("cek halaman");
    if (currentPage == 0)
    {
        update_page_0();
    }
    else if (currentPage == 1)
    {
        update_page_1();
    }
    else if (currentPage == 2)
    {
        update_page_2();
    }
    else if (currentPage == 3)
    {
        update_page_3();
    }
    else if (currentPage == 4)
    {
        update_page_4();
    }
    else if (currentPage == 13)
    {
        update_page_13();
    }
}

String update_timestamp()
{
    String timestamp = update_rtc_year() + "/";
    timestamp += update_rtc_month() + "/";
    timestamp += update_rtc_day() + ", ";
    timestamp += update_rtc_hour() + ":";
    timestamp += update_rtc_minute() + ":";
    timestamp += update_rtc_second();
    return timestamp;
}

String update_payload()
{
    String payload = update_rtc_unix() + String(";");
    payload += aht_data(1) + ";" + aht_data(0) + String(";;");
    payload += String(value1) + String(";") + String(value2) + String(";") + String(value3) + String(";");
    payload += String(temp1) + String(";") + String(temp2) + String(";") + String(temp3) + String(";");
    payload += String(hum1) + String(";") + String(hum2) + String(";") + String(temp3) + String(";");
    return payload;
}

void run_sensor_1()
{
    if (millis() >= time_1 + slcDev1)
    {
        time_1 += slcDev1;
        timestampDev1 = update_timestamp();
        timestampAvg = timestampDev1;

        nextion_separator();
        Serial.print("tProgress.txt=\"");
        Serial.print("Requesting data to Device-1 . . .\""); 
        nextion_separator();

        Serial_Soft.print("a1");
        listen_modbus();
        
        if (sdCardState == 1)
        {
            Serial.print("tProgress.txt=\"");
            Serial.print("Saving to SD card . . .\"");
            nextion_separator();

            String filePath = String("/") + update_rtc_year() + update_rtc_month() + update_rtc_day() + ".csv";
            logSDCard(filePath, update_payload());

            Serial.print("tProgress.txt=\"");
            Serial.print("Data saved to SD card . . .\"");
            nextion_separator();
        }

        Serial.print("tProgress.txt=\"");
        Serial.print("Push data to cloud . . .\"");
        nextion_separator();
        // upload_to_gsheet(value1, value2, value3);
        // makeIFTTTRequest();
    }
}

void run_sensor_2()
{
    if(millis() >= time_2 + slcDev2)
    {
        time_2 += slcDev2;
        timestampDev2 = update_timestamp();
        timestampAvg = timestampDev2;

        nextion_separator();
        Serial.print("tProgress.txt=\"");
        Serial.print("Requesting data to Device-2 . . .\""); 
        nextion_separator();

        Serial_Soft.print("a2");
        listen_modbus();

        if (sdCardState == 1) {
            Serial.print("tProgress.txt=\"");
            Serial.print("Saving to SD card . . .\"");
            nextion_separator();

            String filePath = String("/") + update_rtc_year() + update_rtc_month() + update_rtc_day() + ".csv";
            logSDCard(filePath, update_payload());

            Serial.print("tProgress.txt=\"");
            Serial.print("Data saved to SD card . . .\"");
            nextion_separator();
        }

        Serial.print("tProgress.txt=\"");
        Serial.print("Push data to cloud . . .\"");
        nextion_separator();
        // upload_to_gsheet(value1, value2, value3);
        // makeIFTTTRequest();
    }
}

void run_sensor_3()
{
    if(millis() >= time_3 + slcDev3){
        time_3 += slcDev3;
        timestampDev3 = update_timestamp();
        timestampAvg = timestampDev3;

        Serial.print("tProgress.txt=\"");
        Serial.print("Requesting data to Device-3 . . .\""); 
        nextion_separator();

        Serial_Soft.print("a3");
        listen_modbus();

        if (sdCardState == 1)
        {
            Serial.print("tProgress.txt=\"");
            Serial.print("Saving to SD card . . .\"");
            nextion_separator();

            String filePath = String("/") + update_rtc_year() + update_rtc_month() + update_rtc_day() + ".csv";
            logSDCard(filePath, update_payload());

            Serial.print("tProgress.txt=\"");
            Serial.print("Data saved to SD card . . .\"");
            nextion_separator();
        }

        Serial.print("tProgress.txt=\"");
        Serial.print("Push data to cloud . . .\"");
        nextion_separator();
        // upload_to_gsheet(value1, value2, value3);
        // makeIFTTTRequest();
        write_influx(value3, temp3, hum3, 3);
    }
}

void collect_data()
{
    if (stateMaster == 1 && state1 == 1 && state2 == 1 && state3 == 1)
    {
        run_sensor_1();
        run_sensor_2();
        run_sensor_3();
        averageValue = (value1+value2+value3)/3;
    }
    else if (stateMaster == 1 && state1 == 1 && state2 == 1 && state3 == 0)
    {
        run_sensor_1();
        run_sensor_2();
        averageValue = (value1+value2)/2;
    }
    else if (stateMaster == 1 && state1 == 1 && state2 == 0 && state3 == 1)
    {
        run_sensor_1();
        run_sensor_3();
        averageValue = (value1+value3)/2;
    }
    else if (stateMaster == 1 && state1 == 0 && state2 == 1 && state3 == 1)
    {
        run_sensor_2();
        run_sensor_3();
        averageValue = (value2+value3)/2;
    }
    else if (stateMaster == 1 && state1 == 1 && state2 == 0 && state3 == 0)
    {
        run_sensor_1();
        averageValue = value1;
    }
    else if (stateMaster == 1 && state1 == 0 && state2 == 1 && state3 == 0)
    {
        run_sensor_2();
        averageValue = value2;
    }
    else if (stateMaster == 1 && state1 == 0 && state2 == 0 && state3 == 1)
    {
        run_sensor_3();
        averageValue = value3;
    }
    else if (stateMaster == 1 && state1 == 0 && state2 == 0 && state3 == 0)
    {
        nextion_separator();
        Serial.print("tProgress.txt=\"");
        Serial.print("No device connected\""); 
        nextion_separator();
    }
    else
    {
        nextion_separator();
        Serial.print("tProgress.txt=\"");
        Serial.print("Day night, time to sleep . . . \""); 
    }
}

void init_peripheral()
{
    nextion_separator();
    Serial.print("tLog1.txt=\"");
    Serial.print("Serial communication established\"");
    nextion_separator();
    delay(1000);

    init_aht();
    delay(1000);

    nextion_separator();
    Serial.print("tLog2.txt=\"");
    Serial.print("Initializing SD card . . .\"");
    nextion_separator();
    delay(1000);

    init_sd();
    if (sdCardState == true)
    {
        properties_sd();
    }
    delay(2000);

    int n_try = 5;
    init_modbus();

    // check slave-1 connection
    nextion_separator();
    Serial.print("tLog7.txt=\"");
    Serial.print("Establishing modbus RS485 communication (Device-1) . . .\"");
    nextion_separator();

    for (int i=0; i <= n_try; i++)
    {
        Serial_Soft.print("t1");
        listen_modbus();
        if (state1 == 1)
        {
            Serial.print("tLog8.txt=\"");
            Serial.print("[OK] Slave-1 communication established\"");
            break;
        }
        else
        {
            Serial.print("tLog8.txt=\"");
            Serial.print("[ERROR] Slave-1 communication failed\"");
        }
        nextion_separator();
    }
    nextion_separator();
    delay(1000);

    // check slave-2 connection
    nextion_separator();
    Serial.print("tLog9.txt=\"");
    Serial.print("Establishing modbus RS485 communication (Device-2) . . .\"");
    nextion_separator();

    for (int i=0; i <= n_try; i++)
    {
        Serial_Soft.print("t2");
        listen_modbus();
        if (state2 == 1)
        {
            Serial.print("tLog10.txt=\"");
            Serial.print("[OK] Slave-2 communication established\"");
            break;
        }
        else
        {
            Serial.print("tLog10.txt=\"");
            Serial.print("[ERROR] Slave-2 communication failed\"");
        }
        nextion_separator();
    }
    nextion_separator();
    delay(1000);

    // check slave-3 connection
    nextion_separator();
    Serial.print("tLog11.txt=\"");
    Serial.print("Establishing modbus RS485 communication (Device-3) . . .\"");
    nextion_separator();

    for (int i=0; i <= n_try; i++)
    {
        Serial_Soft.print("t3");
        listen_modbus();
        if (state3 == 1)
        {
            Serial.print("tLog12.txt=\"");
            Serial.print("[OK] Slave-3 communication established\"");
            break;
        }
        else
        {
            Serial.print("tLog12.txt=\"");
            Serial.print("[ERROR] Slave-3 communication failed\"");
        }
        nextion_separator();
    }
    nextion_separator();
    delay(1000);





    init_rtc();
}