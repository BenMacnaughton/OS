shell: shell.o shellmemory.o interpreter.o
	gcc -o shell shell.o shellmemory.o interpreter.o

shell.o: shell.c
	gcc -c shell.c

shellmemory.o: shellmemory.c
	gcc -c shellmemory.c

interpreter.o: interpreter.c
	gcc -c interpreter.c

clean:
	rm *o shell
