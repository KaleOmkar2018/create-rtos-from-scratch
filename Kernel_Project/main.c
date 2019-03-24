#include "osKernel.h"

#define MS_QUANTA (10) //No of ms
//#define COOP
#define PERIODIC
#define NORMAL_SEM
#define RENDEZ_SEM

uint32_t count0 = 0;
uint32_t count1 = 0;
uint32_t count2 = 0;
uint32_t count3 = 0;
uint32_t count4 = 0;

int32_t semaphore_1, semaphore_2;

void task0(void)
{
	while(1)
	{
		count0++;
		
		#ifdef COOP
			osThreadYield();
		#endif
	}
}

void task1(void)
{
	while(1)
	{
		#ifdef NORMAL_SEM
		
		osSemaphoreSignal(&semaphore_1);
		count1++;
		osSemaphoreWait(&semaphore_2);
		#endif
		
		#ifdef RENDEZ_SEM
		osSemaphoreSignal(&semaphore_2);
		osSemaphoreWait(&semaphore_1);
		count1++;
		#endif
	}
}

void task2(void)
{
	while(1)
	{
		#ifdef NORMAL_SEM
		
		osSemaphoreSignal(&semaphore_2);
		count2++;
		osSemaphoreWait(&semaphore_1);
		
		#endif
		
		#ifdef RENDEZ_SEM
		osSemaphoreSignal(&semaphore_2);
		osSemaphoreWait(&semaphore_1);
		count2++;
		#endif
		
	}
}

void Task4(void)
{
		count4++;
}

void Task3(void)
{
		count3++;
}

int main()
{
	#ifdef NORMAL_SEM
	osSemaphoreInit(&semaphore_1, 1);
	osSemaphoreInit(&semaphore_2, 0);
	#endif
	
	#ifdef RENDEZ_SEM
	osSemaphoreInit(&semaphore_1, 0);
	osSemaphoreInit(&semaphore_2, 0);
	#endif
	
	osKernelAddThread(0, task0);
	osKernelAddThread(1, task1);
	osKernelAddThread(2, task2);
	
	osKernelLaunch(MS_QUANTA);
	
	while(1)
	{
		
	}
	return 0;
}
