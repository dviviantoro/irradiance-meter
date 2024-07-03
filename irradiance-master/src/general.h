#include <Arduino.h>

extern bool state1, state2, state3, stateMaster;
extern int currentPage;
extern int slcDev1, slcDev2, slcDev3;
extern int slcStart, slcEnd;
extern int preset1, preset2, preset3, preset4, preset5, preset6, preset7, preset8;
extern int start1, start2, start3, start4, start5;
extern int end1, end2, end3, end4, end5;
extern unsigned long time_1, time_2, time_3, time_4;
extern int averageValue;
extern String timestampDev1, timestampDev2, timestampDev3, timestampAvg;

void check_page();
void run_sensor_1();
void run_sensor_2();
void run_sensor_3();
void collect_data();
void init_peripheral();
void nextion_separator();