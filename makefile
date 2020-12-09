sysprak-client: Main.c performConnection.o config.o
	gcc -o sysprak-client Main.c performConnection.o config.o

performConnection.o: performConnection.c performConnection.h
	gcc -c performConnection.c

config.o: config.c
	gcc -c config.c

clean:
	rm performConnection.o config.o