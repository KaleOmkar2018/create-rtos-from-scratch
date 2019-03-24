		AREA | .text | ,CODE,READONLY,ALIGN=2
		THUMB
		EXTERN	currentTcbPtr
		EXPORT	SysTick_Handler
		EXPORT	osSchedulerLaunch
		IMPORT  osSchedulerRR		;Use this function to make the stack pointer to the required thread.

SysTick_Handler						
	
	CPSID 		I 					
	PUSH		{R4-R11}			
	
	LDR 		R0,=currentTcbPtr	;Save location of the current stack pointer
	LDR			R1,[R0]				;Load value into R1 from address pointed by R0
	STR			SP,[R1]				;Save SP in the running thread to the currentPtr or the address stored in R1.
	;In First Handler, we are using R1 to store the next pointer.
	;This R1 is then stored in the address pointed by R0 register which is currentTcbPtr.
	;So, we need to comment 2 instructions which do the following command: currentTcbPtr = currentTcbPtr->next 
	;1;LDR 		R1,[R1, #4]			;Load the address of the next currentTcbPtr-> r1 = currentPt->next
	;2;STR			R1,[R0]				;Store next Ptr into CurrentTcbPtr
	
	PUSH {R0, LR}					;Why Push, so that when the function returns, it should point to LR ???
	BL osSchedulerRR 				;Use This Function which can be modified as per the scheduling we want.
	
	LDR			SP,[R1]				;Load Value at R1 into SP, this is actual context switching as this shows where the stack will point to.
	
	POP			{R4-R11}			;Why Pop??: Is It necessary??
	CPSIE		I					;Enable Interrupts
	
	BX 			LR					;Restore all the values.

osSchedulerLaunch
	
	LDR			R0,=currentTcbPtr
	LDR			R2,[R0]
	LDR			SP,[R2]

	POP			{R4-R11}			
	POP			{R0-R3}
	POP			{R12}
	ADD			SP,SP,#4
	POP			{LR}
	ADD			SP,SP,#4
	
	CPSIE		I					
	BX 			LR
	
	ALIGN
	END
	