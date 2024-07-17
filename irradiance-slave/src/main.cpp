#include <Arduino.h>
#include <Wire.h>
#include <INA226.h>
#include <Adafruit_AHTX0.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

// #include <InfluxDbClient.h>
// #include <InfluxDbCloud.h>

// #define DEVICE "Slave-3"
// #define LOCATION "Lab"
// #define INFLUXDB_URL "http://192.168.1.203:8086"
// #define INFLUXDB_TOKEN "29znIj8QXqVeX8RdLrftrw10HKeyer428DYq7n5kaMvQK6ZBS9eIEcEhaDfjh1JQF2K0mmieQtOJA00UXUQHZw=="
// #define INFLUXDB_ORG "power-hv"
// #define INFLUXDB_BUCKET "irradiance"

// InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
// Point sensor("irradiance_logger");

int wifiState = 0;
int ssidState = 0;
const int led =  2;
AsyncWebServer server(80);
const char* KNOWN_SSID[] = {"Galaxy-Deny", "INFORMATION CENTER", "TP-Link_5FBA"};
const char* KNOWN_PASSWORD[] = {"bismillah", "9000000000", "68043636"};
const int   KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]);
unsigned long previousMillis = 0;
unsigned long interval = 5000;
unsigned long previousMillis_db = 0;
unsigned long interval_db = 10000;
unsigned long ota_progress_millis = 0;

int Min;
int Max;
int Analog;
unsigned long Sum;
unsigned long Average;

// const int slaveNumber = 1;const char* testCode = "t1";const char* askCode = "a1";String msgOTA = "Hi! I am slave device 1 of irradiance meter system.";
// const int slaveNumber = 2;const char* testCode = "t2";const char* askCode = "a2";String msgOTA = "Hi! I am slave device 2 of irradiance meter system.";
const int slaveNumber = 3;const char* testCode = "t3";const char* askCode = "a3";String msgOTA = "Hi! I am slave device 3 of irradiance meter system...";

#define RXD2 16
#define TXD2 17

INA226 ina(Wire);
Adafruit_AHTX0 aht;

const int numReadings = 100;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int averageValue = 0;            // the average

void initWifi()
{
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
        }
        if (wifiFound) break; // break from the "for each visible network" loop
    }

    if (!wifiFound)
    {
        wifiState = 0;
        Serial.println(F("no Known network identified. Reset to try again"));
    }
    else
    {
        wifiState = 1;

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
        else
        {
            ssidState = 4;
        }

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }

        // SUCCESS, you are connected to the known WiFi network
        Serial.println(F("WiFi connected , your IP address is "));
        Serial.println(WiFi.localIP());
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
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/plain", msgOTA);
    });

    ElegantOTA.begin(&server);    // Start ElegantOTA
    // ElegantOTA callbacks
    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onProgress(onOTAProgress);
    ElegantOTA.onEnd(onOTAEnd);

    server.begin();
    Serial.println("HTTP server (OTA) started");
}


// void init_influx()
// {
//     sensor.addTag("device", DEVICE);
//     sensor.addTag("location", LOCATION);

//     if (client.validateConnection()) {
//         Serial.print("Connected to InfluxDB: ");
//         Serial.println(client.getServerUrl());
//     } else {
//         Serial.print("InfluxDB connection failed: ");
//         Serial.println(client.getLastErrorMessage());
//     }
// }

void setup()
{
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Wire.begin();

    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);

    if(! ina.begin())
    {
        Serial.println("INA226 Error");
        while(1);
    }
    Serial.println("INA226 Initialized");

    if (! aht.begin())
    {
        Serial.println("AHT10 Error");
        while (1);
    }
    Serial.println("AHT10 Initialized");

    ina.configure(INA226_AVERAGES_1, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_CONT);
    ina.calibrate(0.01, 4);   // Calibrate INA226. Rshunt = 0.01 ohm, Max excepted current = 4A
    ina.enableOverPowerLimitAlert();  // Enable Power Over-Limit Alert
    ina.setPowerLimit(0.130);
    ina.setAlertLatch(true);

    for (int thisReading = 0; thisReading < numReadings; thisReading++)
    {
        readings[thisReading] = 0;
    }

    initWifi();
    initOTA();
    // init_influx();
}

void loop()
{
    ElegantOTA.loop();

    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);

    int busVoltage, busPower, shuntVoltage, shuntCurrent;

    busVoltage = ina.readBusVoltage() * 1000000;
    busPower = ina.readBusPower() * 1000000;
    shuntVoltage = ina.readShuntVoltage() * 1000000;
    shuntCurrent = ina.readShuntCurrent() * 1000000;

    total = total - readings[readIndex];
    readings[readIndex] = shuntCurrent;
    total = total + readings[readIndex];
    readIndex = readIndex + 1;
    if (readIndex >= numReadings)
    {
        readIndex = 0;
    }

    averageValue = total / numReadings;

    int x = averageValue;

    double realValue = 0.00502*(pow(x,0.925));

    digitalWrite(led,LOW);
    while(Serial2.available())
    {
        String a = Serial2.readString();
        Serial.println(a);
        
        if (a == testCode)
        {
            digitalWrite(led,HIGH);
            String msg = "rt" + String(slaveNumber) + ";" + String(1);
            Serial2.print(msg);
            Serial.println("masuk test");
        }
        else if (a == askCode)
        {
            digitalWrite(led,HIGH);
            String msg = "ra" + String(slaveNumber) + ";" + String(realValue) + ";" + String(temp.temperature) + ";" + String(humidity.relative_humidity);
            // String msg = "ra" + String(slaveNumber) + ";" + String(averageValue) + ";" + String(temp.temperature) + ";" + String(humidity.relative_humidity);
            // String msg = "ra" + String(slaveNumber) + ";" + String(Average) + ";" + String(temp.temperature) + ";" + String(humidity.relative_humidity);
            Serial2.print(msg);
            Serial.println("masuk test");
        }
    }
    delay(10);

    unsigned long currentMillis = millis();
    // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval))
    {
        Serial.print(millis());
        Serial.print("  ");
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        previousMillis = currentMillis;
    }

    String msg = "ra" + String(slaveNumber) + ";" + String(averageValue) + ";" + String(temp.temperature) + ";" + String(humidity.relative_humidity);
    Serial.println(msg);

    // if (currentMillis - previousMillis_db >= interval_db)
    // {
    //     sensor.clearFields();
    //     sensor.addField("irradiance", realValue);
    //     sensor.addField("temp", temp.temperature);
    //     sensor.addField("hum", humidity.relative_humidity);

    //     Serial.print("Writing: ");
    //     Serial.println(client.pointToLineProtocol(sensor));
  
    //     if (!client.writePoint(sensor))
    //     {
    //         Serial.print("InfluxDB write failed: ");
    //         Serial.println(client.getLastErrorMessage());
    //     }
    
    //     previousMillis_db = currentMillis;
    // }

}