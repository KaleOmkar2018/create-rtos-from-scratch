#include "svcExceptions.h"

//Name of the weak handler declared in startup_stm32f411xe.s
//The same name has to be used as we are overriding 
__asm void SVC_Handler(void)
{
	//Check which Stack Pointer is used.
	TST LR, #4
	
	//Use If-Then Block and use EQUAL as the condition
	ITE EQ
	
	//Check if third bit is equal to zero or not 
	MRSEQ R0, MSP //Copy Address in MSP
	MRSNE R0,	PSP	//Copy Address in PSP
	
	//Call CMSIS-Specific SVC_Handler which is written in C.
	B __cpp(SVC_Handler_C)
}

//CMSIS Standard Function Definition: We are overriding the 
//svc_args: array of the stack frame is passed so that we can get the index into the system call system table.
void SVC_Handler_C(unsigned int* svc_args)
{
	unsigned int svc_number;
	
	//Store the system call system table index
	svc_number = ((char*)svc_args[6]) [-2];
	
	switch(svc_number)
	{
	  case 0:
				svc_args[0] = svc_args[0] + svc_args[1]; //R0 = R0 + R1
				break;
		case 1:
				svc_args[0] = svc_args[0] - svc_args[1]; 
			  break;
		case 2:
				svc_args[0] = svc_args[0] * svc_args[1];
				break;
		case 3:
				svc_args[0] = svc_args[0] / svc_args[1];
				break;
		default:
				break;
	}
}
