#include <SoftwareSerial.h>

extern int value1, value2, value3;
extern float temp1, temp2, temp3;
extern float hum1, hum2, hum3;
extern EspSoftwareSerial::UART Serial_Soft;

void init_modbus();
void listen_modbus();