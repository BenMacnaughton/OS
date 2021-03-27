// Programmer: Ben Macnaughton, 260831379
// Purpose: To immitate the operation of the cpu

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cpu.h"
#include "ram.h"
#include "interpreter.h"
#include "shell.h"
#include "pcb.h"
#include "memorymanager.h"

//external data structures
extern struct CPU* cpu;
extern char *ram[40];
struct Node* head;
struct Node* tail;

//method to run the cpu for a specifed quanta length
//with the active pcb
int run(int quanta, struct PCB *pcb){
    cpu->IP = pcb->PC;
    cpu->offset = pcb->PC_offset;
    int pos = cpu->offset + cpu->IP;
    if(ram[pos]) strcpy(cpu->IR, ram[pos]);
    else{
        handleFault(pcb);
        return -1;
    }

    int i;
    for(i = 0; i < quanta; i++){
        char *args[5];
        //parse input for interpreter
        parse(cpu->IR, ' ', args);
        interpreter(args);
        cpu->offset ++;
        pos = cpu->IP + cpu->offset;
        if(ram[pos] != NULL) strcpy(cpu->IR, ram[pos]);
        else strcpy(cpu->IR, " ");
        if(!strcmp(cpu->IR, " ") || cpu->offset == 4) {
            int finished = handleFault(pcb);
            if(finished) return -1; //program is finished
            return -2; //program is still running
        }
    }
    return cpu->offset;
}

//method to handle a page fault
//returns -1 if the program has finished
//returns 0 otherwise
int handleFault(struct PCB *pcb){
    pcb->PC_page++;
    int page = pcb->PC_page;
    if(page == pcb->pages_max) return 1; //program is finished
    if(pcb->pageTable[page] == -1){

        int frame = findFrame();
        //if none found find victim frame and resolve pcbs
        if(frame == -1){
            frame = findVictim(pcb);
            resolveVictims(frame);
        }
        updateRAM(page, frame, pcb->name);
        updatePageTable(pcb, page, frame);
    }
    pcb->PC = pcb->pageTable[page]*4;
    pcb->PC_offset = 0;
    return 0;
}

//method to add a pcb to the ready queue
void addToReady(struct PCB *newPCB){
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = newPCB;
    newNode->next = NULL;
    if(!head){
        head = newNode;
        tail = newNode;
    }else{
        //add to tail
        tail->next = newNode;
        tail = newNode;
    }
}