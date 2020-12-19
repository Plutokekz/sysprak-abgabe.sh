sysprak-client-tests: testing.c config.o
	gcc -g -o sysprak-client-tests testing.c config.o

config.o: ../config.c ../config.h
	gcc -c ../config.c

clean:
	rm testing.o config.o