#include <modbus-ao.h>
#include <general.h>

#define timeWait 3000
#define RXD2 16
#define TXD2 17

int value1 = 0, value2 = 0, value3 = 0;
float temp1 = 0, temp2 = 0, temp3 = 0;
float hum1 = 0, hum2 = 0, hum3 = 0;

void init_modbus()
{
    Serial2.setTimeout(250);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length()-1;

    for(int i=0; i<=maxIndex && found<=index; i++)
    {
        if(data.charAt(i)==separator || i==maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1]+1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void listen_modbus()
{
    unsigned long startedWaiting = millis();
    while(Serial2.available() <= 0 && millis() - startedWaiting <= timeWait)
    {
        nextion_separator();
        Serial.print("tProgress.txt=\"");
        Serial.print("Listening for slave response . . .\""); 
        nextion_separator();

        String str = Serial2.readString();
        Serial.println(str);
        String part01 = getValue(str,';',0);
        String part02 = getValue(str,';',1);
        ////////////
        String part03 = getValue(str,';',2);
        String part04 = getValue(str,';',3);
        ////////////
        
        if (part01 == "rt1" && part02 == "1")
        {
            state1 = 1;
            break;
        }
        else if (part01 == "rt2" && part02 == "1")
        {
            state2 = 1;
            break;
        }
        else if (part01 == "rt3" && part02 == "1")
        {
            state3 = 1;
            break;
        }

        if (part01 == "ra1")
        {
            value1 = part02.toInt();
            temp1 = part03.toFloat();
            hum1 = part04.toFloat();
            break;
        }
        else if (part01 == "ra2")
        {
            value2 = part02.toInt();
            temp2 = part03.toFloat();
            hum2 = part04.toFloat();
            break;
        }
        else if (part01 == "ra3")
        {
            value3 = part02.toInt();
            temp3 = part03.toFloat();
            hum3 = part04.toFloat();
            break;
        }
    }
}