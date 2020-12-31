EFLAGS = -Wall -Wextra -Werror

sysprak-client: Main.c performConnection.o config.o
	gcc $(EFLAGS) -o sysprak-client Main.c performConnection.o config.o

performConnection.o: performConnection.c performConnection.h
	gcc $(EFLAGS) -c performConnection.c

config.o: config.c
	gcc $(EFLAGS) -c config.c

clean:
	rm performConnection.o config.o
