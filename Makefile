EFLAGS = -Werror -g

sysprak-client: testSHM.c performConnection.o config.o shareMemory.o
	gcc $(EFLAGS) -o sysprak-client testSHM.c performConnection.o config.o shareMemory.o

performConnection.o: performConnection.c performConnection.h
	gcc $(EFLAGS) -c performConnection.c

config.o: config.c
	gcc $(EFLAGS) -c config.c

shareMemory.o: shareMemory.c
	gcc $(EFLAGS) -c shareMemory.c

play:
	./sysprak-client

vg:
	valgrind --leak-check=full --trace-children=yes ./sysprak-client

clean:
	rm performConnection.o config.o shareMemory.o
