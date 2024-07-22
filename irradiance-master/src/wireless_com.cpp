#include <wireless_com.h>
#include <sensor-ao.h>
#include <rtc-ao.h>
#include <general.h>
#include <modbus-ao.h>

#define DEVICE "TTGO-T7-V15"
#define LOCATION "SMP IT Wasilah Garut"

// local db test
/*
#define INFLUXDB_URL "http://192.168.1.203:8086"
#define INFLUXDB_TOKEN "29znIj8QXqVeX8RdLrftrw10HKeyer428DYq7n5kaMvQK6ZBS9eIEcEhaDfjh1JQF2K0mmieQtOJA00UXUQHZw=="
#define INFLUXDB_ORG "power-hv"
#define INFLUXDB_BUCKET "irradiance"
*/
#define INFLUXDB_URL "http://167.205.76.2:8086"
#define INFLUXDB_USER "deny"
#define INFLUXDB_PASSWORD "12345678"
#define INFLUXDB_DB_NAME "irradiance_logger"


#define BOTtoken "5917398507:AAFSCyGgvUH9LoeGT4Ch47d8yE3uMThglfw"  // your Bot Token (Get from Botfather)
#define CHAT_ID "-898094308"    //group irradiance
// #define CHAT_ID "691646481"   //saya

const char* KNOWN_SSID[] = {"Galaxy-Deny", "information_center", "INFORMATION CENTER", "TP-Link_5FBA", "smpitwasilah"};
const char* KNOWN_PASSWORD[] = {"bismillah", "9000000000", "9000000000", "68043636", "cipanas15"};
const int   KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); // number of known networks

bool wifiState = false;
int ssidState = 0;
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
unsigned long ota_progress_millis = 0;
String msgOTA = "Hi! I am master device! (new version)";

// InfluxDBClient client_db(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
InfluxDBClient client_db(INFLUXDB_URL, INFLUXDB_DB_NAME);
Point sensor("irradiance_logger");

AsyncWebServer server_ota(80);
WiFiClientSecure client_tele;
UniversalTelegramBot bot(BOTtoken, client_tele);

void init_influxdb()
{
    client_db.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DB_NAME, INFLUXDB_USER, INFLUXDB_PASSWORD);
    sensor.addTag("device", DEVICE);
    sensor.addTag("location", LOCATION);

    // Check server connection
    if (client_db.validateConnection()) {
        Serial.print("Connected to InfluxDB: ");
        Serial.println(client_db.getServerUrl());
    } else {
        Serial.print("InfluxDB connection failed: ");
        Serial.println(client_db.getLastErrorMessage());
    }
}

void onOTAStart()
{
    // Log when OTA has started
    Serial.println("OTA update started!");
    // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final)
{
    // Log every 1 second
    if (millis() - ota_progress_millis > 1000)
    {
        ota_progress_millis = millis();
        Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
    }
}

void onOTAEnd(bool success) {
    // Log when OTA has finished
    if (success) {
        Serial.println("OTA update finished successfully!");
    } else {
        Serial.println("There was an error during OTA update!");
    }
    // <Add your own code here>
}

void initOTA()
{
    server_ota.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/plain", msgOTA);
    });

    ElegantOTA.begin(&server_ota);    // Start ElegantOTA
    // ElegantOTA callbacks
    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onProgress(onOTAProgress);
    ElegantOTA.onEnd(onOTAEnd);

    server_ota.begin();
    Serial.println("HTTP server (OTA) started");
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

        initOTA();
        Serial.print("tLog18.txt=\"");
        Serial.print("[OK] OTA connected\"");
        nextion_separator();

        // init_gsheet();
        initOTA();
        init_influxdb();
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

void write_influx(int irradiance, float temp, float humidity, int node)
{
    sensor.clearFields();
    String field_irr = "irr_" + String(node);
    String field_temp = "temp_" + String(node);
    String field_hum = "hum_" + String(node);

    sensor.addField(field_irr, irradiance);
    sensor.addField(field_temp, temp);
    sensor.addField(field_hum, humidity);
  
    Serial.print("\nWriting: ");
    Serial.println(client_db.pointToLineProtocol(sensor));

    // Write point
    if (!client_db.writePoint(sensor))
    {
        Serial.print("\nInfluxDB write failed: ");
        Serial.println(client_db.getLastErrorMessage());
    }
    else
    {
        Serial.println("\nInfluxDB write sucess . . .");
    }
}