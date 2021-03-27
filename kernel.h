#ifndef KERNEL
#define KERNEL
#include "pcb.h"
int myinit(char *filename);
void addToReady(struct PCB *pcb);
int scheduler();
#endif