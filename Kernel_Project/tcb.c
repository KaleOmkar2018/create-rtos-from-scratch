#define NUM_OF_THREADS 4
#define STACK_SIZE 100 //Each stack element is 4-byte wide. So, we will store 400 bytes.

#include "stdint.h"

typedef struct tcb{
	int32_t* stackPt;
	struct tcb* nextTcbPt;
}tcbType;

tcbType noOfTcbs[NUM_OF_THREADS]; //Create an array of threads
tcbType* currentPtr; //Pointer To Current TCB
int32_t tcbStacks[NUM_OF_THREADS][STACK_SIZE]; //Create stacks for all the threads.