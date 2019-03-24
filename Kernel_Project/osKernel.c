#include "osKernel.h"

void osSchedulerLaunch(void);

#define NUM_OF_THREADS (3)
#define STACK_SIZE (100) //100 words: yup
#define STACK_PTR_ADDR (STACK_SIZE - 16)
#define EPSR (STACK_SIZE - 1)
#define PC (STACK_SIZE - 2)

#define BUS_FREQ (16000000)
#define ONE_MS (1000)

//Important Registers
#define SYSPRI3 (*((volatile uint32_t*)0xE000ED20)) 
#define INTCTL	(*((volatile uint32_t*)0xE000ED04)) //Required to call Systick

uint32_t freqDivider = 1;
uint32_t periodic_tick = 0; //This tells the scheduler when to switch tasks.
#define PERIOD 100

//#define SINGLE_TASK 
//#define TWO_TASK

#define POLLING_SEM
//#define COOP_SEM

//Need to add a bit which says that this stack is filled or not.
typedef struct tcb{
	int32_t* stackPtr;
	struct tcb* nextPtr;
}tcb;

tcb tcbs[NUM_OF_THREADS]; //Array Of Three Threads
tcb *currentTcbPtr; //Pointer To Current TCB
int32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE]; //Array of stacks for threads

//Functions for Internal use
//Stack Setup: Pass Thread Number(starting from zero to N - 1)
static bool osKernelStackInit(uint8_t threadNo)
{
	bool retVal = false;
	
	//Check if argument passed is less than no of threads allowed
	if(threadNo < NUM_OF_THREADS)
	{
		//Pt to the global stack pointer assigned to this thread
		//Stack Size is 100 words. So, 83rd word is where it will point(84th from 1st)
		tcbs[threadNo].stackPtr = &TCB_STACK[threadNo][STACK_SIZE - 16];
		
		//Use Thumb State: 24th bit of EPSR: Last One Word of the stack contains EPSR(99th word)
		TCB_STACK[threadNo][STACK_SIZE - 1] = 0x01000000;
		
		retVal = true;
	}
	
	return retVal;
}

//Add Single Threads and Their Functions 
int8_t osKernelAddThread( uint8_t threadNum, void (*task)(void))
{
	//Critical Section
	__disable_irq();
	
	//Point to Next Element if it is not the last thread
	if(threadNum < (NUM_OF_THREADS - 1))
	{
		tcbs[threadNum].nextPtr = &tcbs[threadNum + 1];
	}
	//Point To 0th element if the index is last
	else if(threadNum == (NUM_OF_THREADS - 1))
	{
		tcbs[threadNum].nextPtr = &tcbs[0];
		currentTcbPtr = &tcbs[0]; //Hack, make the currentTcbPtr point to stack pointer of 0th element.
	}
	else
	{
		//Halt the Process: It should not reach here for now.
		return FAILURE;
	}
	
	//Come Here Only If A Valid Index is Passed	
	osKernelStackInit(threadNum); //Check Not Added
	
	//Program Counter Should Point to Function That the Task Executes.
	TCB_STACK[threadNum][STACK_SIZE - 2] = (int32_t)(task);
	
	__enable_irq();
	
	return SUCCESS;
}

uint32_t getSysTickFreq(void)
{
	return (BUS_FREQ / ONE_MS);
}

void osKernelLaunch(uint32_t quanta)
{
	__disable_irq();
	
	//Get the Value To be Set for SysTick Timer
	freqDivider = getSysTickFreq();
	
	//Disable Counter
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	
	
	//SysTick Default Priority -> -1
	//Set Priority of SysTick to E0
	//PendSV priority: -2 (used for context switch)
	SYSPRI3 = ( (SYSPRI3 & 0x00FFFFFF) | 0xE0000000);
	
	//Load Value for countdown
	SysTick->LOAD = ( (quanta * freqDivider) - 1);
	
	//Start the counter: enable counter, get tick interrupt, Clock Source is processor clock
	SysTick->CTRL = 0x00000007;
	
	osSchedulerLaunch();
		
	//__enable_irq(); //We do not enable irqs because we do that using assembly!!
}

void osThreadYield(void)
{
	INTCTL = 0x04000000; //Trigger Systick Directly -> Call Handler -> Contex Switch
}

void osSemaphoreInit(int32_t* semaphore, int32_t value)
{
	*semaphore = value;
}

void osSemaphoreSignal(int32_t* semaphore)
{
	__disable_irq();
	
	*semaphore += 1;
	
	__enable_irq();	
}

void osSemaphoreWait(int32_t* semaphore)
{
	__disable_irq();
	
	//Wait till semaphore is available. It is available when semaphore value is greater than zero.
	while(*semaphore <= 0)
	{
		#ifdef POLLING_SEM
		__disable_irq();
		
		#ifdef COOP_SEM
		osThreadYield();
		#endif
		
		__enable_irq();
		#endif
	}
	
	*semaphore -= 1;
	
	__enable_irq();	
}

void osSchedulerRR(void)
{
	//Run this task only when PERIOD has elapsed.
	#ifdef SINGLE_TASK
		if(++periodic_tick == PERIOD)
		{	
			//execute this function
			(*Task3)();
		}
	#endif
	#ifdef TWO_TASK
		if(++periodic_tick % 100)
		{
			(*task3)();
		}
		
		if(++periodic_tick % 250)
		{
			(*task4)();
		}
	#endif
	currentTcbPtr = currentTcbPtr->nextPtr; //This is normal RR scheduler but implemented in C.
}
