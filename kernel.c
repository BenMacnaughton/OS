// Programmer: Ben Macnaughton, 260831379
// Purpose: To impliment all OS kernel functions

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shell.h"
#include "interpreter.h"
#include "pcb.h"
#include "ram.h"
#include "kernel.h"
#include "cpu.h"
#include "memorymanager.h"


//ready queue nodes
extern struct Node* head;
extern struct Node* tail;
//external data structures
extern struct CPU* cpu;
extern char *ram[40];

//method declarations
int kernel();
void boot();

int main(){
    int error = 0;
    boot();
    error = kernel();
    return error;
}

void boot(){
    int i;
    //set to zero pages
    for(i = 0; i < 40; i++) ram[i] = NULL;
    //create store if it does not exist
    char* com = "if [ ! -d ./BackingStore ]; then mkdir BackingStore; fi";
    system(com);
    //clear store
    com = "rm -rf BackingStore/*";
    system(com);
}

int kernel(){
    //initialize ready queue nodes
    head = NULL;
    tail = NULL;
    //initialize cpu
    cpu = (struct CPU*)malloc(sizeof(struct CPU));
    cpu->quanta = 2;
    printf("Kernel 1.0 loaded!\n");
    return shellUI();
}

int myinit(char *filename){
    //open file
    FILE *script = fopen(filename, "r");
    //error check
    if(!script){
        printf("Program %s was not found.\n", filename);
        return 1;
    }
    int launched = launcher(script, filename);
    if(launched) return 0;
    printf("The program %s was too large to launch\n", filename);
    return 1;
}

struct Node* getNext(){
    //remove and return head
    struct Node* next = head;
    head = head->next;
    return next;
}

int scheduler(){
    while(head){
        struct Node* next = (struct Node*)getNext();
        //return next instruction to be executed
        int n = run(cpu->quanta, next->data);
        if(n == -2){ //a page fault occured and the program should still be running
            addToReady(next->data);
        }else if(n == -1){ //a page fault occured and the program has finished
            free(next->data);
        }else{//otherwise update PC and add to back of queue
            next->data->PC_offset = n;
            addToReady(next->data);
        }
        free(next);
    }
    return 0;
}