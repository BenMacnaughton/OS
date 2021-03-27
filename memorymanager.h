#ifndef MEMORYMANAGER
#define MEMORYMANAGER
#include <stdio.h>
#include "pcb.h"
int launcher(FILE *p, char *name);
int countTotalPages(FILE *f);
void loadPage(int pageNumber, FILE *f, int frameNumber);
int findFrame();
int findVictim(struct PCB *p);
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber);
int isBlank(char *line);
int checkPCB(struct PCB *p, int n);
int checkActivePages(int n);
void resolveVictims(int frame);
void updateRAM(int page, int frame, char* name);
#endif