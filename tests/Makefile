CFLAGS=-Wall -Wextra -Werror -g
RUN=-1
export RUN

ifeq ($(RUN),1)
sysprak-client-tests-pipe: testCmdPipe.c cmdPipe.o
	gcc $(CFLAGS) -o sysprak-client-tests-pipe testCmdPipe.c cmdPipe.o

cmdPipe.o: ../module/cmdPipe.c ../module/cmdPipe.h
	gcc -c ../cmdPipe.c

clean:
	rm *.o
endif

ifeq ($(RUN),0)
sysprak-client-tests-config: testConfig.c config.o
	gcc -g -o sysprak-client-tests-config testConfig.c config.o

config.o: ../module/config.c ../module/config.h
	gcc -c ../config.c

clean:
	rm *.o
endif

ifeq ($(RUN),2)
sysprak-client-tests-shm: testSHM.c shareMemory.o
	gcc -g -o sysprak-client-tests-shm testSHM.c shareMemory.o

shareMemory.o: ../module/shareMemory.c ../module/shareMemory.h
	gcc -c ../shareMemory.c
endif

ifeq ($(RUN),-1)
clean:
	rm *.o
endif