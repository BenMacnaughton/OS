#ifndef CPU_
#define CPU_
#include "pcb.h"
struct CPU {int IP;
int offset;
char IR[1000];
int quanta;
};
struct CPU* cpu;
int run(int quanta, struct PCB *pcb);
struct Node {
    struct PCB *data;
    struct Node *next;
};
struct Node* head;
struct Node* tail;
void addToReady(struct PCB *newPCB);
int handleFault(struct PCB *pcb);
#endif