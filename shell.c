// Programmer: Ben Macnaughton, 260831379
// Purpose: To impliment the user facing shell of the OS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "interpreter.h"
#include "shell.h"

void clean(char *string);

int shellUI(){
	printf("Welcome to the Ben shell!\nVersion 2.0 created February 2020\n");
 	char in[256];
	char *args[5];
	if(isatty(STDIN_FILENO)) printf("$ ");
	while(fgets(in, sizeof(in), stdin)){
		parse(in, ' ', args);
		if(args[0]){
			interpreter(args);
		}
		if(isatty(STDIN_FILENO)) printf("$ ");
	}
	return 0;
}

void parse(char *string, char delimiter, char *tokens[]) {
	clean(string);
	char buffer[1000];
	int s=0, b=0, t=0; // counters for string, buffer and tokens

	while(*(string+s) != '\0' && *(string+s)!='\n'){
		// skip leading spaces
		for(;*(string+s)==' ';s++);

		// exit if end of string
		if(*(string+s) == '\0') break;

		// otherwise, add word to buffer
		b=0;
		while(*(string+s)!=delimiter && *(string+s)!='\0') {
			buffer[b] = *(string+s);
			s++;
			b++;
		}
		buffer[b]='\0'; // make it a string

		// create a token
		tokens[t] = strdup(buffer);
		t++;
	}

	for(; t < 5; t++) tokens[t] = NULL;
}

void clean(char *string){
	int len = strlen(string);
    string[len] = '\n';
    string[len+1] = '\0';
    char* strc = string;
	char charc;
	while(*strc != '\n' &&  *strc != '\0'){
        ++strc;
    }
    if(*strc){
        *strc = '\0';
    }else{
        while((charc = getchar()) != '\n' && charc != EOF) continue;
    }
}