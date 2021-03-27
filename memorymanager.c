// Programmer: Ben Macnaughton, 260831379
// Purpose: To impliment all memory managment functions for the OS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "memorymanager.h"
#include "pcb.h"
#include "cpu.h"
#include "ram.h"

//external structs and data
extern char *ram[40];
extern struct Node* head;
extern struct Node* tail;

//method to launch a file, copy its contents into a backup
//and load its first two pages into ram
int launcher(FILE *p, char* name){
    char path[128] = "./BackingStore/";
    strcat(path, name);
    //open the file from the backup
    FILE *backup = fopen(path, "w+");
    char line[256];
    //copy all non blank lines to backup
    while (fgets(line, sizeof(line), p)) {
        if(!isBlank(line)) fprintf(backup, "%s", line);
    }
    //close original file
    fclose(p);
    int pages = countTotalPages(backup);
    if(pages > 10) return 0; //too large

    struct PCB *newPCB = makePCB(pages, name);
    int frame;
    int i;
    //add up to 2 pages to ram
    for(i = 0; i < 2 && i < pages; i++){
        frame = findFrame();
        if(frame == -1){
            frame = findVictim(newPCB);
            resolveVictims(frame);
        }

        if(!i) newPCB->PC = frame*4; //initialize PC if first iteration
        loadPage(i, backup, frame);
        updatePageTable(newPCB, i, frame);
    }
    fclose(backup);
    addToReady(newPCB); //add new pcb to ready queue

    return 1; //return success
}

//method to count the number of pages needed for a file
int countTotalPages(FILE *f){
    int lines = 0;
    char line[256];
    //reset the line pointer
    rewind(f);
    //copy all non blank lines to backup
    while (fgets(line, sizeof(line), f)){
        if(!isBlank(line)) lines++;
    }
    return (lines-1)/4 + 1;
}

//method to load a page of data into ram
void loadPage(int pageNumber, FILE *f, int frameNumber){
    //clear the old frame
    removeFromRAM(frameNumber);
    //add contents of new page to ram
    addToRAM(f, pageNumber, frameNumber);
}

//method to find an empty frame in ram
//returns frame number or -1 if none found
int findFrame(){
    int p = 0;
    while(ram[p] != NULL && p < 40) p += 4;
    if(p < 40) return p/4;
    return -1;
}

//method to find a victim frame to be overwritten
int findVictim(struct PCB *p){
    int i = rand()%10;
    //check that frame is not active or being used by the current pcb
    while(checkActivePages(i) || checkPCB(p, i)) i = (i+1)%10;
    return i;
}

//method to update the page table of a specified pcb
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber){
    p->pageTable[pageNumber] = frameNumber;
    return 0;
}

//method to detirmine if a line contains only whitespace
//returns -1 if not blank, 0 if blank
int isBlank(char *line){
    char *ch;
    int blank = -1;
    for (ch = line; *ch != '\0'; ++ch){
        if (!isspace(*ch)){
            blank = 0;
            break;
        }
    }
    return blank;
}

//method to check if a victim frame is needed by the active pcb
int checkPCB(struct PCB *p, int n){
    int valid = 0;
    int i;
    for(i = p->PC_page; i < p->pages_max; i++){
        if(p->pageTable[i] == n) valid = -1;
    }
    return valid;
}

//method to check if a victim frame is the active
//frame of any pcbs in the ready queue
int checkActivePages(int n){
    int valid = 0;
    struct Node *node = head;
    while(node != NULL){
        int active = head->data->PC_page;
        if(head->data->pageTable[active] == n) valid = 1;
        node = node->next;
    }
    return valid;
}

//method to reset the page table of any pcbs
//affected by a victim frame
void resolveVictims(int frame){
    struct Node *node = head;
    while(node != NULL){
        struct PCB *pcb = node->data;
        int i;
        for(i = 0; i < 10; i++){
            if(pcb->pageTable[i] == frame){
                pcb->pageTable[i] = -1;
                break;
            }
        }
        node = node->next;
    }
}

//wrapper method for load page
void updateRAM(int page, int frame, char* name){
    FILE *f = fopen(name, "r");
    if(!f) {
        printf("The file could not be opened\n");
        exit(1);
    }
    loadPage(page, f, frame);
    fclose(f);
}