// Programmer: Ben Macnaughton, 260831379
// Purpose: To interpret user inputs and execute the appropriate function

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "shellmemory.h"
#include "kernel.h"
#include "memorymanager.h"

//helper signatures
void help();
void quit();
int set(char* var, char* val);
int print(char* var);
int runScript();
int exec(char *first, char *second, char *third);

//function to interpret user command
int interpreter(char *args[]){
    char* com = args[0];
    if(!strcmp(com, "help")){
        if(!args[1]){
            help();
            return 0;
        }else{
            printf("Incorrect usage.\n");
            return 1;
        }
    }else if(!strcmp(com, "quit")){
        if(!args[1]){
            quit();
            return 0;
        }else{
            printf("Incorrect usage.\n");
            return 1;
        }
    }else if(!strcmp(com, "set")){
        if(!args[3]) return set(args[1], args[2]);
        else{
            printf("Incorrect usage.\n");
            return 1;
        }
    }else if(!strcmp(com, "print")){
        if(!args[2]) return print(args[1]);
        else{
            printf("Incorrect usage.\n");
            return 1;
        }
    }else if(!strcmp(com, "run")){
        if(!args[2]) return runScript(args[1]);
        else{
            printf("Incorrect usage.\n");
            return 1;
        }
    }else if(!strcmp(com, "exec")){
        if(!args[4]) return exec(args[1], args[2], args[3]);
        else{
            printf("Too many programs. Max 3.\n");
            return 1;
        }
    }else{
        printf("Unknown command.\n");
        return 1;
    }
}

void help(){
    printf("COMMAND\t\t\t\tDESCRIPTION\n");
    printf("help\t\t\t\tDisplays all the commands\n");
    printf("quit\t\t\t\tExits / terminates the shell with “Bye!”\n");
    printf("set VAR STRING\t\t\tAssigns a value to shell memory\n");
    printf("print VAR\t\t\tDisplays the STRING assigned to VAR\n");
    printf("run SCRIPT.TXT\t\t\tExecutes the file SCRIPT.TXT\n");
    printf("exec prog1 prog2 prog3\t\tExecutes up to 3 concurrent programs provided as arguments\n");
}

void quit(){
    printf("Bye!\n");
    exit(0);
}

int set(char* var, char* val){
    if(var && val){
        save(var, val);
        return 0;
    }else{
        //error
        printf("Please enter a variable and a value.\n");
        return 1;
    }
}

int print(char* var){
    if(var){
        char *val = find(var);
        if(strcmp(val, "")){
            printf("%s\n", val);
        }
        return 0;
    }else{
        //error
        printf("Please enter a variable.\n");
        return 1;
    }
}

int runScript(char* file){
    if(file){
        FILE *script = fopen(file, "r");
        if(script){
            char line[256];
            char *args[3];
            while (fgets(line, sizeof(line), script)){
                parse(line, ' ', args);
                if(args[0] && interpreter(args)){
                    printf("An error in the script occured.\n");
                    return 1;
                }
            }
            fclose(script);
        }else{
            printf("Script not found.\n");
            return 1;
        }
        return 0;
    }else{
        //error
        printf("Please enter a script.\n");
        return 1;
    }
}

int exec(char *first, char *second, char *third){
    if(!first){
        printf("Please enter at least one program\n");
        return 1;
    }
    //if error return
    if(myinit(first)) return 1;;
    //process second program
    if(second){
        //if error return
        if(myinit(second)) return 1;;
    }
    //process third program
    if(third){
        //if error return
        if(myinit(third)) return 1;
    }
    //if all were successful, call scheduler
    return scheduler();
}