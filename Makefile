EFLAGS = -Wall -Wextra -Werror -g
G=0000000000000
C=tests/config.conf
export C
export G

sysprak-client: Main.c performConnection.o config.o shareMemory.o thinker.o
	gcc $(EFLAGS) -o sysprak-client Main.c performConnection.o config.o shareMemory.o thinker.o

performConnection.o: performConnection.c performConnection.h
	gcc $(EFLAGS) -c performConnection.c

config.o: config.c
	gcc $(EFLAGS) -c config.c

shareMemory.o: shareMemory.c shareMemory.h
	gcc $(EFLAGS) -c shareMemory.c

thinker.o: thinker.c thinker.h
	gcc $(EFLAGS) -c thinker.c

cmdPipe.o: modules/cmdPipe.c modules/cmdPipe.h
	gcc $(EFLAGS) -c modules/cmdPipe.c

gamePhase.o: gamePhase.c gamePhase.h
	gcc $(EFLAGS) -c gamePhase.c

play:
	./sysprak-client
play: sysprak-client
	./sysprak-client -g $(GAME_ID) -p $(PLAYER)

vg:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./sysprak-client -g $(G) -c $(C)

clean:
	rm performConnection.o config.o shareMemory.o thinker.o
