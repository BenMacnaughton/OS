#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

void parse(char *string, char delimiter, char *tokens[]);
void clean(char *string);

int main(){
	printf("Welcome to the Ben shell!\nVersion 1.0 created January 2020\n");
 	char in[256];
	char *args[4];
	if(isatty(STDIN_FILENO)) printf("$ ");
	while(fgets(in, sizeof(in), stdin)){
		parse(in, ' ', args);
		if(args[0]){
			interpreter(args);
		}
		if(isatty(STDIN_FILENO)) printf("$ ");
	}
}

void parse(char *string, char delimiter, char *tokens[]) {
	clean(string);
	char buffer[1000];
	int s=0, b=0, t=0; // counters for string, buffer and tokens

	while(*(string+s) != '\0'){
		// skip leading spaces
		for(;*(string+s)==' ';s++);

		// exit if end of string
		if(*(string+s) == '\0') break;

		// otherwise, add word to buffer
		b=0;
		while(*(string+s)!=delimiter && *(string+s)!='\0' && *(string+s)!='\n') {
			buffer[b] = *(string+s);
			s++;
			b++;
		}
		buffer[b]='\0'; // make it a string

		// create a token
		tokens[t] = strdup(buffer);
		t++;
	}
	tokens[t] = NULL;
}

void clean(char *string){
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
