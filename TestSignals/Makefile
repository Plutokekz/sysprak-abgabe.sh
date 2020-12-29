sys: testSignal.c shareMemory.o 
	gcc -g -o sysprak-client testSignal.c shareMemory.o

shareMemory.o: shareMemory.c shareMemory.h
	gcc -g -c shareMemory.c

play:
	./sysprak-client

vg:
	valgrind --leak-check=full --trace-children=yes ./sysprak-client
	
clean:
	rm shareMemory.o
