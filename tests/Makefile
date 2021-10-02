CFLAGS=-Wall -Wextra -Werror -g
RUN=-1
export RUN

test-queue: testQueue.c queue.o
	gcc $(CFLAGS) -o queue testQueue.c queue.o
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./queue

queue.o: ../modules/queue.h ../modules/queue.c
	gcc $(CFLAGS) -c ../modules/queue.c

test-bitboard: testBitboard.c bitboard.o log.o
	gcc $(CFLAGS) -o bitboard testBitboard.c bitboard.o log.o
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./bitboard

bitboard.o: ../modules/bitboard.h ../modules/bitboard.c
	gcc $(CFLAGS) -c ../modules/bitboard.c

log.o: ../modules/log.h ../modules/log.c
	gcc $(CFLAGS) -c ../modules/log.c

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
	rm *.o queue bitboard
endif
