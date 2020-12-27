sys: testSHM.c shareMemory.o 
	gcc -o sysprak-client testSHM.c shareMemory.o

shareMemory.o: shareMemory.c shareMemory.h
	gcc -c shareMemory.c

play:
	./sysprak-client


clean:
	rm shareMemory.o
