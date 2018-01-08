#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "system.h"

#define SYSTICK_FREQUENCY_1000HZ ((uint32_t)1000)

void vSystickInit(uint32_t ulFrequency);
uint32_t ulGetSystickNowInMs(void);
void vDelaySystickInMs(uint32_t ulMs);

#endif
