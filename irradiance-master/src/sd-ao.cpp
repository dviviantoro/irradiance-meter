#include <sd-ao.h>
#include <general.h>
#define SD_CS 5

bool sdCardState = false;

void properties_sd()
{
    Serial.print("tLog4.txt=\"");
    uint8_t cardType = SD.cardType();
    if(cardType == CARD_NONE)
    {
        Serial.print("No SD card attached\"");
        return;
    }
    Serial.print("Initializing SD card type . . .\"");
    nextion_separator();

    Serial.print("tLog5.txt=\"");
    switch (cardType)
    {
        case CARD_MMC:
            Serial.print("SD Card type is MMC\"");
            break;
        case CARD_SD:
            Serial.print("SD Card type is SDSC\"");
            break;
        case CARD_SDHC:
            Serial.print("SD Card type is SDHC\"");
            break;
        default:
            Serial.print("SD Card type is unknown\"");
            break;
    }
    nextion_separator();
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.print("tLog6.txt=\"");
    Serial.printf("SD card size: %lluMB\n\"", cardSize);
    nextion_separator();
}

void init_sd()
{
    if (SD.begin())
    {
        sdCardState = 1;
        Serial.print("tLog3.txt=\"");
        Serial.print("[OK] SD card mount success\"");
        nextion_separator();
    }
    else if (!SD.begin())
    {
        Serial.print("tLog3.txt=\"");
        Serial.print("[ERROR] SD card failed\"");
        nextion_separator();
    }
}

void appendFile(fs::FS &fs, const char * path, const char * message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

void logSDCard(String filePath, String payload) {
    appendFile(SD, filePath.c_str(), (payload + "\r\n").c_str());
}