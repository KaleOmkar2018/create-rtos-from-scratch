#include "sysTickTimer.h"

volatile uint32_t tick;
volatile uint32_t _tick;

void setClockFrequency(uint32_t divider)
{
	//Notify the Core that we are updating the clock
	SystemCoreClockUpdate();
	
	//Update Frequency:  For f Hz, time will be 1 s, so for m / 1000 Hz, it will be 1 ms.
	SysTick_Config(SystemCoreClock / divider);
	
	//Enable interrupts
	__enable_irq();
}

//Standard function. We do not need to declare it in our header file.
//This results in creation of another function which will never be called.
//Just define it and it has weak definition.
//Commented due to multiple definition error
/*
void SysTick_Handler(void)
{
	++tick;
}
*/
uint32_t getTick(void)
{
	//We can just return the value!.
	__disable_irq();
	_tick = tick; //Critical Section: Block It
	__enable_irq();
	
	return _tick;
}

void delayInSeconds(uint32_t noOfSeconds)
{
	//CPU freq is 16Mhz and we are setting it at 16M/100. So, we need to multiply it by 100.
	noOfSeconds *= 100;
	
	//Get the Current Tick Value
	uint32_t firstTickValue = getTick();
	
	//Delay Loop
	while( (getTick() - firstTickValue) < noOfSeconds )
	{
		//Wait till delay is completed
	}
}
