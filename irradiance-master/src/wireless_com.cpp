#include <wireless_com.h>
#include <sensor-ao.h>
#include <rtc-ao.h>
#include <general.h>
#include <modbus-ao.h>

#define PROJECT_ID "irradiance-logger-425801"
#define CLIENT_EMAIL "irradiance-logger-service@irradiance-logger-425801.iam.gserviceaccount.com"

#define BOTtoken "5917398507:AAFSCyGgvUH9LoeGT4Ch47d8yE3uMThglfw"  // your Bot Token (Get from Botfather)
#define CHAT_ID "-898094308"    //group irradiance
// #define CHAT_ID "691646481"   //saya

const char* KNOWN_SSID[] = {"Galaxy-Deny", "INFORMATION CENTER", "TP-Link_5FBA", "smpitwasilah"};
const char* KNOWN_PASSWORD[] = {"bismillah", "9000000000", "68043636", "cipanas15"};
const int   KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); // number of known networks

const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDQbh0mMV0uiXPQ\nNzJGlImczXG3N4axxYy8SNDFVSmsw1hK5uYO93vwzvHD2jv0zxJ+mvRf5ZmwZQCT\nmMo36ZzHWc8KuTILtPCkpPUW8U7keaikfgHANel60k7jVlv/wtU5xeGKKtSLmObJ\nHpVdTGm4+qx1or0BivRyOpn0wpiYVlhJi9xgMr7opp663b+v6U4o37NctWkJr1np\nWKozXdITPlx5aHxsi08Ren4ZlCx8LWDmDagBzIAy1L/DeEEvprWVPA5AMzf1kKvF\nqmFAfSGUCWOB0FaL/pyBfNFcB8kV9Sb+xgxogyXbE5XRrFA4u73Edmc2zrdkDIA/\nwP3nkhUlAgMBAAECggEAGUevGmb0sIR0Zbs5dQJmxHwjQD/TAvR9KDMZZe3OjbPt\nyBFEXb6vq4VLdoqdfgkFlbyGn6unSJP043IDM4/j95vmzEaqZsLDSJJHYHPT3UtS\nOeKcpMBajrk7tJV7YsSHUdK0kypfDYV17HaHUuxwiu1Ahfi/PZTUfEGrqFpX4WtT\nS0CwaIJithCb8cKgZwOFC0aqup1Pwd5BsMtwnDIc/gYZp79Iylp/vc6M3O0Ju/r9\nDCB/khuXmfnjdD8eGRsLeL0xblUYgXLg0+J59FmhLlz4n9/sCEuCy1xJTASQ8CSh\n0hh9rc2d/ifztuRS4D9PD5vgy9rF6cE82T3TrfqPuwKBgQD+59VdEvqCii4/pjUH\ndx4Rx/fSviiYtXHRMZAYKlFArayS5/+Y/5Zwm+9LwP/TfwgwHt8yVqptcEoJ4tDl\nd54dR+qKOD33QCwfsftpejUYbK+3tkOKN6rlPZ8N7PXC0IeL5mjHjctPYiBNryk6\npGxPZ5rKUbfv+/1cD+H5yqpCewKBgQDRUzMC21BRnEd+P+NQK6gHCTD8HprodM+X\nd3KkgEjYnEoyzPSFhk3Vv5IOcqXma7vDaz4fGcsAxqt+N0T5WAieiRUBhrhMI+ft\njlHGOHGK+0xe0omFgb8t0yioolHlT/fNidviINDN0lx8npb5PbwRbJ0pxGBOq7P1\nH7209lTE3wKBgQCYzXSThKry0c6DjSjAvuEhm5OtmxxO3oXJ90H/mm52rSWEMtp/\nZsdR3cgGyz0QFVhdMERvspAbZnKbloHO7HO17cRk8u4CZ0+L+G9TfVSSvaRgJjw6\ngMBLBzNvb5qrG8IFVSt+ZDt57MKcjuHSK3pZ5ytdGLuAergYFdVKnfiOPQKBgB3O\nbxEVEZbNXhDIICImVULLOEJuSTiYiYbT52stKTJO7vHU5Wyt9x9cq/uKr7zj0lS9\nTJc5IQL3RywtypCGzJNmiasx98Q/yKK3BUXoe6KCrhlCeXqdQw/PZZa9plO002Bd\nJQZKDfGA5tKH8o+DDuicQrTT1bLeGD3rSyQdf2yZAoGATLs5CYX6PssNTRmqvA0Y\nMVCiT5hEkl6JFeKQtZHlWfoEaFIZm/5/poWfDmehCZdUR3M5JebirX4DjW0FvVEY\nQCqZoCCCVYPb5xMOUa75qIqvf5U1N2RCirN6P49zh+9YvXBV/UuLIezhFltOcy7s\nrKe4d/AFxpPyP678sdIgzyQ=\n-----END PRIVATE KEY-----\n";
const char spreadsheetId[] = "1dMt2qyTz7lXJs7lLNZ-yuvO9SkYED_QHo8KKps-_wFU";

bool wifiState = false;
int ssidState = 0;
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

AsyncWebServer server(80);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void tokenStatusCallback(TokenInfo info);

void init_gsheet()
{
    GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);
    GSheet.setTokenCallback(tokenStatusCallback);
    GSheet.setPrerefreshSeconds(10 * 60);
    GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
}

void initWifi()
{
    Serial.print("tLog15.txt=\"");
    Serial.print("Start scan Wi-Fi connection . . .\"");
    nextion_separator();

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

    if (nbVisibleNetworks == 0)
    {
        Serial.println(F("no networks found. Reset to try again"));
        while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
    }

    // if you arrive here at least some networks are visible
    Serial.print(nbVisibleNetworks);
    Serial.println(" network(s) found");

    // check if we recognize one by comparing the visible networks one by one with our list of known networks
    for (i = 0; i < nbVisibleNetworks; ++i)
    {
        Serial.println(WiFi.SSID(i)); // Print current SSID
        for (n = 0; n < KNOWN_SSID_COUNT; n++)
        {
            if (strcmp(KNOWN_SSID[n], WiFi.SSID(i).c_str()))
            {
                Serial.print(F("\tNot matching "));
                Serial.println(KNOWN_SSID[n]);
            }
            else
            {
                wifiFound = true;
                break; // n is the network index we found
            }
        } // end for each known wifi SSID
        if (wifiFound) break; // break from the "for each visible network" loop
    } // end for each visible network

    if (!wifiFound)
    {
        wifiState = false;
        Serial.println(F("no Known network identified. Reset to try again"));

        nextion_separator();
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
        wifiState = true;

        Serial.print(F("\nConnecting to "));
        Serial.println(KNOWN_SSID[n]);

        // We try to connect to the WiFi network we found
        WiFi.begin(KNOWN_SSID[n], KNOWN_PASSWORD[n]);
        
        if (KNOWN_SSID[n] == "Galaxy-Deny")
        {
            ssidState = 1;
        }
        else if (KNOWN_SSID[n] == "INFORMATION CENTER")
        {
            ssidState = 2;
        }
        else if (KNOWN_SSID[n] == "TP-Link_5FBA")
        {
            ssidState = 3;
        }
        else if (KNOWN_SSID[n] == "smpitwasilah")
        {
            ssidState = 4;
        }
        else
        {
            ssidState = 5;
        }

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial2.print(".");
        }
        Serial2.println("");
        
        nextion_separator();
        Serial.print("tLog16.txt=\"");
        Serial.print("[OK] Wi-Fi connected, your SSID is ");
        Serial.print(KNOWN_SSID[n]);
        Serial.print("\"");
        nextion_separator();

        // SUCCESS, you are connected to the known WiFi network
        Serial2.println(F("WiFi connected , your IP address is "));
        Serial2.println(WiFi.localIP());

        Serial.print("tLog17.txt=\"");
        Serial.print("[OK] Wi-Fi connected, your IP address is ");
        Serial.print(WiFi.localIP());
        Serial.print("\"");
        nextion_separator();

        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            request->send(200, "text/plain", "Hi! I am master device!");
        });

        AsyncElegantOTA.begin(&server);
        server.begin();

        Serial.print("tLog18.txt=\"");
        Serial.print("[OK] OTA connected\"");
        nextion_separator();

        init_gsheet();
    }
}

void handleNewMessages(int numNewMessages)
{
    Serial.println("handleNewMessages");
    Serial.println(String(numNewMessages));

    for (int i=0; i<numNewMessages; i++)
    {
        String chat_id = String(bot.messages[i].chat_id);
    
        if (chat_id != CHAT_ID)
        {
            bot.sendMessage(chat_id, "Unauthorized user", "");
            continue;
        }
        
        // Print the received message
        String text = bot.messages[i].text;
        Serial.println(text);

        String from_name = bot.messages[i].from_name;

        if (text == "/start")
        {
            String welcome = "Welcome, ";
            welcome += from_name;
            welcome += ".\n";
            welcome += "This is telegram group service for Irradiance Meter IRM.V01 by Pesawat Sederhana\n\n";
            welcome += "Please use the following commands to control your outputs:\n";
            welcome += "/start to request command list \n";
            welcome += "/stat to request device status list \n";
            welcome += "/device1 to request information of device1 \n";
            welcome += "/device2 to request information of device2 \n";
            welcome += "/device3 to request information of device3 \n";
            welcome += "/master to request information of master device \n";
            welcome += "/other to request other information \n";
            bot.sendMessage(chat_id, welcome, "");
        }

        if (text == "/stat")
        {
            String messageTelegram = "Status slave device: \n\n";
            state1 == 1 ? messageTelegram += "Device1 is ONLINE\n" : messageTelegram += "Device1 is OFFLINE\n";
            state2 == 1 ? messageTelegram += "Device2 is ONLINE\n" : messageTelegram += "Device2 is OFFLINE\n";
            state3 == 1 ? messageTelegram += "Device3 is ONLINE\n" : messageTelegram += "Device3 is OFFLINE\n";
            bot.sendMessage(chat_id, messageTelegram, "");
        }

        if (text == "/device1")
        {
            String messageTelegram = "Device1 current infomation: \n\n";
            if (state1 == 1)
            {
                messageTelegram += "Device1 is ONLINE\n";
                messageTelegram += "Sampling time = " + String(slcDev1) + " ms\n";
                messageTelegram += "Last value = " + String(value1) + " Watt/m^2 at " + String(timestampDev1) + "\n";
                messageTelegram += "Internal temperature = " + String(temp1) + " C\n";
                messageTelegram += "Internal RH = " + String(hum1) + " %\n";
            }
            else
            {
                messageTelegram += "Device1 is OFFLINE\n";
            }
            bot.sendMessage(chat_id, messageTelegram, "");
        }

        if (text == "/device2")
        {
            String messageTelegram = "Device2 current infomation: \n\n";
            if (state2 == 1)
            {
                messageTelegram += "Device2 is ONLINE\n";
                messageTelegram += "Sampling time = " + String(slcDev2) + " ms\n";
                messageTelegram += "Last value = " + String(value2) + " Watt/m^2 at " + String(timestampDev2) + "\n";
                messageTelegram += "Internal temperature = " + String(temp2) + " C\n";
                messageTelegram += "Internal RH = " + String(hum2) + " %\n";
            }
            else
            {
                messageTelegram += "Device2 is OFFLINE\n";
            }
            bot.sendMessage(chat_id, messageTelegram, "");
        }

        if (text == "/device3")
        {
            String messageTelegram = "Device3 current infomation: \n\n";
            if (state3 == 1)
            {
                messageTelegram += "Device3 is ONLINE\n";
                messageTelegram += "Sampling time = " + String(slcDev3) + " ms\n";
                messageTelegram += "Last value = " + String(value3) + " Watt/m^2 at " + String(timestampDev3) + "\n";
                messageTelegram += "Internal temperature = " + String(temp3) + " C\n";
                messageTelegram += "Internal RH = " + String(hum3) + " %\n";
            }
            else
            {
                messageTelegram += "Device3 is OFFLINE\n";
            }
            bot.sendMessage(chat_id, messageTelegram, "");
        }

        if (text == "/master")
        {
            // sensors_event_t humidity, temp;
            // aht.getEvent(&humidity, &temp);
            String messageTelegram = "Master device current infomation: \n\n";
            
            if (ssidState == 1)
            {
                messageTelegram += "Connected to Galaxy-Deny\n";
            }
            else if (ssidState == 2)
            {
                messageTelegram += "Connected to INFORMATION CENTER\n";
            }
            else if (ssidState == 3)
            {
                messageTelegram += "Connected to TP-Link_5FBA\n";
            }
            else if (ssidState == 4)
            {
                messageTelegram += "Connected to smpitwasilah\n";
            }
            // messageTelegram += "IP address: " + String(WiFi.localIP()) + "\n";
            messageTelegram += "Average value = " + String(averageValue) + " Watt/m^2 at " + String(timestampAvg) + "\n";
            messageTelegram += "Internal temperature = " + aht_data(1) + " C\n";
            messageTelegram += "Internal RH = " + aht_data(0) + " %\n";
            messageTelegram += "Woke up at " + String(slcStart) + ".00\n";
            messageTelegram += "Woke up at " + String(slcEnd) + ".00\n";
            bot.sendMessage(chat_id, messageTelegram, "");
        }

        if (text == "/other")
        {
            String messageTelegram = "Enjoy available connection: \n\n";
            messageTelegram += "Connect to telegram group: https://t.me/+ZJZcKTzBEqJiMzg1\n";  
            messageTelegram += "Connect to google charts: http://bit.ly/irradiancelog\n";  
            messageTelegram += "Connect to google spreadsheet: https://docs.google.com/spreadsheets/d/182IbsPoWkU0i1yBsak2M-Sis1zAkhttDBXaFQd_blms/edit?usp=sharing\n\n";  
            
            messageTelegram += "If you have some errors or request, please connect me: \n";  
            messageTelegram += "Telegram: https://telegram.me/d190195\n";  
            messageTelegram += "Whatsapp: https://wa.me/6285741311479\n";  
            messageTelegram += "Email: deny.viviantoro@gmail.com\n";  
            bot.sendMessage(chat_id, messageTelegram, "");
        }
    }
}

void check_incoming_telegram()
{
    if (wifiState == 1)
    {
        if (millis() > lastTimeBotRan + botRequestDelay)
        {
            int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

            while(numNewMessages)
            {
                Serial2.println("got response");
                handleNewMessages(numNewMessages);
                numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            }
            lastTimeBotRan = millis();
        }
    }
}

void tokenStatusCallback(TokenInfo info)
{
    if (info.status == token_status_error)
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
        GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
    }
    else
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    }
}

void upload_to_gsheet(int value1, int value2, int value3)
{
    FirebaseJson response;
    Serial.println("\nAppend spreadsheet values...");
    Serial.println("----------------------------");
    FirebaseJson valueRange;

    String date_now = update_rtc_year() + "/";
    date_now += update_rtc_month() + "/";
    date_now += update_rtc_day();

    String time_now = update_rtc_hour() + ":";
    time_now += update_rtc_minute() + ":";
    time_now += update_rtc_second();

    valueRange.add("majorDimension", "COLUMNS");
    valueRange.set("values/[0]/[0]", update_rtc_unix());
    valueRange.set("values/[1]/[0]", date_now);
    valueRange.set("values/[2]/[0]", time_now);
    valueRange.set("values/[3]/[0]", value1);
    valueRange.set("values/[4]/[0]", value2);
    valueRange.set("values/[5]/[0]", value3);

    bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append */, &valueRange /* data range to append */);
    if (success)
    {
        response.toString(Serial, true);
        valueRange.clear();
    }
    else
    {
        Serial.println(GSheet.errorReason());
    }
    Serial.println();
    Serial.println(ESP.getFreeHeap());
}