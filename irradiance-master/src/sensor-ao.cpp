#include <sensor-ao.h>

Adafruit_AHTX0 aht;

void init_aht()
{
    if (! aht.begin()) {
        Serial.println("AHT10 Error");
    }
    else
    {
        Serial.println("AHT10 Initialized");
    }
}

String aht_data(bool selection)
{
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    String aht_temp = String(temp.temperature);
    String aht_hum = String(humidity.temperature);
    if (selection == true)
    {
        return aht_temp;
    }
    else 
    {
        return aht_hum;
    }
}