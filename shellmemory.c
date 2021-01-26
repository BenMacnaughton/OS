#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

#define memsize 1000 //max memory size

//memory struct
struct MEM { char *var; char *value; };

//memory array
struct MEM vars[memsize];

//hash signature
int hash(char *val);

void save(char* var, char* val){
    int h = hash(var);
    if(vars[h].var){
        if(!strcmp(vars[h].var, var)){
            vars[h].value = val;
        }else{
            printf("Find a better hashing function.\n");
            exit(1);
        }
    }else{
        struct MEM new = {var, val};
        vars[h] = new;
    }
}

char* find(char* var){
    int h = hash(var);
    if(!vars[h].var){
        return "";
    }else{
        return vars[h].value;
    }
}

int hash(char *val){
    int hash = 1453;
    int i;
    while ((i = *val++)) hash = ((hash << 3) + hash) + i;
    return hash % memsize;
}