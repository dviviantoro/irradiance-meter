#include <FS.h>
#include <SD.h>
#include <SPI.h>

extern bool sdCardState;

void init_sd();
void properties_sd();
void logSDCard(String filePath, String payload);