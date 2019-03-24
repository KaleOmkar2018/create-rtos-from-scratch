#ifndef OS_KERNEL_H
#define OS_KERNEL_H

#include "common.h"

//Kernel Functions
void osKernelLaunch(uint32_t quanta);
uint32_t getSysTickFreq(void);
int8_t osKernelAddThread( uint8_t threadNum, void (*task0)(void) );
void osThreadYield(void);
void osSchedulerRR(void); //Instead of switching to next thread in assembly, we are trying to use C for the same purpose.

//Semaphore Functions
void osSemaphoreInit(int32_t* semaphore, int32_t value);
void osSemaphoreSignal(int32_t* semaphore);
void osSemaphoreWait(int32_t* semaphore);


//Periodic Tasks
void task3(void);
void task4(void);

#endif
