#ifndef PCB_
#define PCB_
struct PCB {
    int PC;
    int pageTable[10];
    int PC_page;
    int PC_offset;
    int pages_max;
    char* name;
    };
struct PCB* makePCB(int pages_max, char *name);
#endif