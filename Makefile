EFLAGS = -Wall -Wextra -Werror
G=0000000000000
C=tests/config.conf
export C
export G

sysprak-client: Main.c performConnection.o config.o shareMemory.o
	gcc $(EFLAGS) -o sysprak-client Main.c performConnection.o config.o shareMemory.o

performConnection.o: performConnection.c performConnection.h
	gcc $(EFLAGS) -c performConnection.c

config.o: config.c
	gcc $(EFLAGS) -c config.c

shareMemory.o: shareMemory.c
	gcc $(EFLAGS) -c shareMemory.c

play: sysprak-client
	./sysprak-client -g $(GAME_ID) -p $(PLAYER)

vg:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./sysprak-client -g $(G) -c $(C)

clean:
	rm performConnection.o config.o shareMemory.o
