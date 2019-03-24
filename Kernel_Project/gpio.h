#ifndef GPIO_H
#define GPIO_H

#include "common.h"

typedef enum{
	off = 0,
	on = 1
}ledState;

typedef enum{
	GREEN = (1U << 12),
 	ORANGE = (1U << 13),
	RED = (1U << 14),
	BLUE = (1U << 15)
}LED;

#define GREEN_MODE (1U << 24) //25 = 0, 24 = 1
#define ORANGE_MODE (1U << 26) //27 = 0, 26 = 1
#define RED_MODE (1U << 28) //29 = 0, 28 = 1
#define BLUE_MODE (1U << 30) //31 = 0, 30 = 1

#define GPIODEN (1 << 3)

void gpioInit(void);
void toggleBlue(void);
void setLedBlue(ledState led_on);
void setLedAny(LED color, ledState led_on);

#endif
