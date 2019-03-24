#ifndef SYSTICKTIMER_H
#define SYSTICKTIMER_H

#include "common.h"

void setClockFrequency(uint32_t divider);
uint32_t getTick(void);
void delayInSeconds(uint32_t noOfSeconds);

#endif
