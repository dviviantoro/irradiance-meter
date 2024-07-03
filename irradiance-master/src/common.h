#include <Arduino.h>

unsigned long previousMillis = 0;
unsigned long interval = 10000;

const int ledPin = 2;
bool ledState = LOW;

int dummy = 0;
String dataMessage, payload, filePath;
 
const int adc0 = 36;
const int adc1 = 26;
const int relay0 = 35;
int adc0Val = 0;
int adc1Val = 0;

int relay0State = 0;
int mainPowerState = 0;
int batPowerState = 0;

const char* resource = "/trigger/irradiance/with/key/jto3ra7Ajzi5n2hr_SZ0bCBBKSjmDb-AQ5YRjlC0_Il";
const char* serverIFTTT = "maker.ifttt.com";
const int Enable =  2;