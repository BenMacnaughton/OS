// Programmer: Ben Macnaughton, 260831379
// Purpose: To imitate the ram and allow for writing and removing to/from fram

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shell.h"
#include "interpreter.h"
#include "ram.h"

extern char *ram[40];

void addToRAM(FILE *p, int pageNumber, int frameNumber){
    char line[256];
    int i = 0;
    //rewind and "seek" to desired position
    rewind(p);
    while(i < pageNumber*4 && fgets(line, sizeof(line), p)) i++;
    //copy up to 4 lines into ram at desired frame
    i = 0;
    while(i < 4 && fgets(line, sizeof(line), p)){
        ram[frameNumber*4 + i] = strdup(line);
        i++;
    }
}

//method to remove a frame from ram
void removeFromRAM(int frameNumber){
    //clear frame index-by-index
    int i;
    for(i = 0; i < 4; i++) ram[frameNumber*4 + i] = NULL;
}
