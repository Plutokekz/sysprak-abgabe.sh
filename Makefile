EFLAGS = -Wall -Wextra -Werror -g
G=0000000000000
C=tests/config.conf
export C
export G

sysprak-client: Main.c performConnection.o config.o shareMemory.o thinker.o gamePhase.o cmdPipe.o
	gcc $(EFLAGS) -o sysprak-client Main.c performConnection.o config.o shareMemory.o thinker.o gamePhase.o cmdPipe.o

performConnection.o: performConnection.c performConnection.h
	gcc $(EFLAGS) -c performConnection.c

config.o: modules/config.c
	gcc $(EFLAGS) -c modules/config.c

shareMemory.o: modules/shareMemory.c modules/shareMemory.h
	gcc $(EFLAGS) -c modules/shareMemory.c

thinker.o: thinker.c thinker.h
	gcc $(EFLAGS) -c thinker.c

cmdPipe.o: modules/cmdPipe.c modules/cmdPipe.h
	gcc $(EFLAGS) -c modules/cmdPipe.c

gamePhase.o: gamePhase.c gamePhase.h
	gcc $(EFLAGS) -c gamePhase.c

play:
	./sysprak-client

vg:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./sysprak-client -g $(G) -c $(C)

clean:
	rm performConnection.o config.o shareMemory.o thinker.o cmdPipe.o gamePhase.o
