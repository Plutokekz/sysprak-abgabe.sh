EFLAGS = -Wall -Wextra -Werror -g
G=0000000000000
C=tests/config.conf
P=1
export C
export G

sysprak-client: Main.c performConnection.o utils.o shareMemory.o thinker.o gamePhase.o cmdPipe.o bitboard.o protocolUtils.o
	gcc $(EFLAGS) -o sysprak-client Main.c performConnection.o utils.o shareMemory.o thinker.o gamePhase.o cmdPipe.o bitboard.o protocolUtils.o

performConnection.o: performConnection.c performConnection.h
	gcc $(EFLAGS) -c performConnection.c

shareMemory.o: modules/shareMemory.c modules/shareMemory.h
	gcc $(EFLAGS) -c modules/shareMemory.c

cmdPipe.o: modules/cmdPipe.c modules/cmdPipe.h
	gcc $(EFLAGS) -c modules/cmdPipe.c

utils.o: modules/utils.c modules/utils.h
	gcc $(EFLAGS) -c modules/utils.c

gamePhase.o: gamePhase.c gamePhase.h
	gcc $(EFLAGS) -c gamePhase.c

thinker.o: thinker.c thinker.h
	gcc $(EFLAGS) -c thinker.c

protocolUtils.o: modules/protocolUtils.c modules/protocolUtils.h
	gcc $(EFLAGS) -c modules/protocolUtils.c

bitboard.o: modules/bitboard.c
	gcc $(EFLAGS) -c modules/bitboard.c

play: sysprak-client
	./sysprak-client -g $(G) -p $(P) -c $(C)

vg:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./sysprak-client -g $(G) -c $(C)

clean:
	rm performConnection.o utils.o shareMemory.o thinker.o gamePhase.o cmdPipe.o bitboard.o protocolUtils.o
