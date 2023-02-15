#ifndef DEVICE_SETTINGS
#define DEVICE_SETTINGS

#include <inttypes.h>

bool initSettings();
bool eraseSettings();
void setIntValue(int32_t value, const char *name);
void setStringValue(const char *value, const char *name);
int32_t getIntValue(const char *name);
char* getStringValue(const char *name);

#endif