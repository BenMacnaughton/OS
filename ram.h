#ifndef RAM
#define RAM
#include <stdio.h>
void addToRAM(FILE *p, int pageNumber, int frameNumber);
char *ram[40];
void removeFromRAM(int frameNumber);
#endif