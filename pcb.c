// Programmer: Ben Macnaughton, 260831379
// Purpose: To impliment the creation of a new PCB

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "interpreter.h"
#include "pcb.h"

//method to create a new pcb
struct PCB* makePCB(int pages_max, char *name){
    struct PCB *newPCB = (struct PCB*)malloc(sizeof (struct PCB));
    newPCB->name = name;
    newPCB->pages_max = pages_max;
    newPCB->PC_page = 0;
    newPCB->PC_offset = 0;
    int i;
    for(i = 0; i < 10; i++) newPCB->pageTable[i] = -1;
    return newPCB;
}