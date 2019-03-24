#include "common.h"
#include "gpio.h"
#include "sysTickTimer.h"

#define FREQUENCY_DIVIDER (100U)

void gpioInit(void)
{
	//Set Clock
	RCC->AHB1ENR |= GPIODEN;
	
	//Set as Output(LEDs)
	GPIOD->MODER |= (GREEN_MODE | ORANGE_MODE | RED_MODE | BLUE_MODE);
	
	//Set Clock frequency to 16 Mhz / 100
	setClockFrequency(FREQUENCY_DIVIDER);
}

void toggleBlue(void)
{
	GPIOD->ODR ^= (BLUE);
}

void setLedBlue(ledState led_on)
{
	if(led_on)
	{
		GPIOD->ODR |= (BLUE);
	}
	else
	{
		GPIOD->ODR &= ~(BLUE);
	}
}

void setLedAny(LED color, ledState led_on)
{
	if(led_on)
	{
		GPIOD->ODR |= (color);
	}
	else
	{
		GPIOD->ODR &= ~(color);
	}
}