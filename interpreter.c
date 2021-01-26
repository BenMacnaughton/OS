#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

//helper signatures
void help();
void quit();
int set(char* var, char* val);
int print(char* var);
int run();

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
        if(!args[2]) return run(args[1]);
        else{
            printf("Incorrect usage.\n");
            return 1;
        }
    }else{
        printf("Unknown command.\n");
        return 1;
    }
}

void help(){
    printf("COMMAND\t\t\tDESCRIPTION\n\n");
    printf("help\t\t\tDisplays all the commands\n");
    printf("quit\t\t\tExits / terminates the shell with “Bye!”\n");
    printf("set VAR STRING\t\tAssigns a value to shell memory\n");
    printf("print VAR\t\tDisplays the STRING assigned to VAR\n");
    printf("run SCRIPT.TXT\t\tExecutes the file SCRIPT.TXT\n");
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
        }else{
            printf("Variable does not exist.\n");
        }
        return 0;
    }else{
        //error
        printf("Please enter a variable.\n");
        return 1;
    }
}

int run(char* file){
    if(file){
        FILE *script = fopen(file, "r");
        if(script){
            char line[256];
            char *args[3];
            while (fgets(line, sizeof(line), script)){
                int len = strlen(line);
                line[len] = '\n';
                line[len+1] = '\0';
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